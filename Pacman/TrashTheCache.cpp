#include "TrashTheCache.h"

test::TrashTheCache::TrashTheCache(diji::GameObject* ownerPtr, int id)
	: Component(ownerPtr)
	, m_ID{ id }
{
	InitializePlot();
}

void test::TrashTheCache::Update()
{
	ImGui::Begin(std::format("Exercise {}", m_ID).c_str());

	ImGui::SetWindowSize(ImVec2{});

	switch (m_ID)
	{
	case 1:
		Exercice1();
		break;
	case 2:
		Exercice2();
		break;
	default:
		break;
	}

	ImGui::End();
}

void test::TrashTheCache::InitializePlot()
{
	m_PlotConfig.values.xs = x_data;
	m_PlotConfig.values.ys = 0;
	m_PlotConfig.values.count = 11;
	m_PlotConfig.values.color = colors[0];
	m_PlotConfig.scale.min = 0;
	m_PlotConfig.scale.max = 0;
	m_PlotConfig.tooltip.show = true;
	m_PlotConfig.tooltip.format = "x=%.2f, y=%.2f";
	m_PlotConfig.grid_x.show = true;
	m_PlotConfig.grid_y.show = true;
	m_PlotConfig.frame_size = ImVec2(200, 100);
	m_PlotConfig.line_thickness = 2.f;

	m_PlotConfig2 = m_PlotConfig;
	m_PlotConfig2.values.color = colors[1];

	m_PlotConfig3 = m_PlotConfig;
	m_PlotConfig3.values.ys = nullptr;
	m_PlotConfig3.values.ys_list = list;
	m_PlotConfig3.values.ys_count = 2;
	m_PlotConfig3.values.colors = colors;
}

void test::TrashTheCache::Exercice1()
{
	ImGui::InputInt("# samples", &m_SampleCount);

	ImGui::BeginDisabled(m_IsFirstBenchmarkDone);
	if (ImGui::Button("Trash the cache"))
	{
		Benchmark<int>(m_SampleCount);

		SetMaxPlotValue(m_PlotConfig, m_Results2);
	
		m_IsFirstBenchmarkDone = true;
	};
	ImGui::EndDisabled();

	if (m_IsFirstBenchmarkDone)
		ImGui::Plot("plot", m_PlotConfig);
}

void test::TrashTheCache::Exercice2()
{
	ImGui::InputInt("# samples", &m_SampleCount);

	ImGui::BeginDisabled(m_IsFirstBenchmarkDone);
	if (ImGui::Button("Trash the cache with GameObject3D"))
	{
		Benchmark<GameObject3D>(m_SampleCount);

		SetMaxPlotValue(m_PlotConfig, m_Results);

		list[0] = m_Results.data();
		m_IsFirstBenchmarkDone = true;
	};
	ImGui::EndDisabled();

	if (m_IsFirstBenchmarkDone)
		ImGui::Plot("GameObject3D", m_PlotConfig);

	ImGui::BeginDisabled(m_IsSecondBenchmarkDone);
	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
	{
		Benchmark<GameObject3DAlt>(m_SampleCount);

		SetMaxPlotValue(m_PlotConfig2, m_Results2);

		list[1] = m_Results2.data();
		m_IsSecondBenchmarkDone = true;
	};
	ImGui::EndDisabled();

	if (m_IsSecondBenchmarkDone)
		ImGui::Plot("GameObject3DAlt", m_PlotConfig2);

	if (m_IsFirstBenchmarkDone and m_IsSecondBenchmarkDone)
	{
		//maybe calculate max value after benchmark and store it in member variable
		const auto maxResult = *std::max_element(m_Results.begin(), m_Results.end());
		const auto maxResult2 = *std::max_element(m_Results2.begin(), m_Results2.end());
		const float maxPlot3 = std::max(maxResult, maxResult2);

		m_PlotConfig3.scale.max = maxPlot3;
		ImGui::Plot("Combined", m_PlotConfig3);
	}
}

void test::TrashTheCache::SetMaxPlotValue(ImGui::PlotConfig& plotConfig, const std::vector<float>& results)
{
	plotConfig.values.ys = results.data();
	const auto maxElement = std::max_element(results.begin(), results.end());

	if (maxElement != results.end())
		plotConfig.scale.max = *maxElement;
}
