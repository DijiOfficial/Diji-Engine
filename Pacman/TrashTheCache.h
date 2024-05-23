#pragma once
#include "GameObject.h"

#include <imgui.h>
#include <imgui_plot.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <format>
// needs threading
namespace test
{
	template<typename T, typename = std::void_t<>>
	struct has_ID_member : std::false_type {};

	template<typename T>
	struct has_ID_member<T, std::void_t<decltype(std::declval<T>().ID)>> : std::true_type {};

	class TrashTheCache final : public diji::Component
	{
	public:
		TrashTheCache(diji::GameObject* ownerPtr, int id);
		~TrashTheCache() noexcept override = default;

		TrashTheCache(const TrashTheCache& other) = delete;
		TrashTheCache(TrashTheCache&& other) = delete;
		TrashTheCache& operator=(const TrashTheCache& other) = delete;
		TrashTheCache& operator=(TrashTheCache&& other) = delete;

		void Init() override {};
		void Update() override;
		void FixedUpdate() override {};
				

	private:
		struct Matrix4x4
		{
			float matrix[16] = { 1, 0, 0, 0,
								 0, 1, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1 };
		};

		struct GameObject3D
		{
			Matrix4x4 transform{};
			int ID{ 1 };
		};

		struct GameObject3DAlt
		{
			Matrix4x4* transform{};
			int ID{ 1 };
		};

		void InitializePlot();
		
		void Exercice1();
		void Exercice2();
		void SetMaxPlotValue(ImGui::PlotConfig& plotConfig, const std::vector<float>& results);
		
		template<typename T>
		void Benchmark(int sampleCount)
		{
			constexpr int count = 25'000'000;
			int stepSize = 1;

			std::vector<T> buffer(count, T{});

			for (stepSize = 1; stepSize <= 1024; stepSize *= 2)
			{
				std::vector<long long> durations;
				for (int i = 0; i < sampleCount + 2; ++i)
				{
					auto start = std::chrono::high_resolution_clock::now();
					for (size_t j = 0; j < count; j += stepSize)
					{
						if constexpr (has_ID_member<T>::value)
						{
							buffer[j].ID *= 2;
						}
						else
						{
							buffer[j] *= 2;
						}
					}
					auto end = std::chrono::high_resolution_clock::now();

					const auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
					durations.push_back(deltaTime);
				}
				const auto minmax = std::minmax_element(durations.begin(), durations.end());
				const auto sum = std::accumulate(durations.begin(), durations.end(), 0LL) - (*minmax.first) - (*minmax.second);
				
				if constexpr (std::is_same_v<T, GameObject3D>)
					m_Results.push_back(static_cast<float>(static_cast<double>(sum) / (static_cast<int>(durations.size()) - 2)));
				else
					m_Results2.push_back(static_cast<float>(static_cast<double>(sum) / (static_cast<int>(durations.size()) - 2)));
			}
		}
	
		int m_SampleCount{ 10 };
		int m_ID;
		bool m_IsFirstBenchmarkDone{ false };
		bool m_IsSecondBenchmarkDone{ false };
		const float* list[2];

		const ImU32 colors[2]{ 0xFFFF0000, 0xFF0000FF };

		ImGui::PlotConfig m_PlotConfig{}, m_PlotConfig2{}, m_PlotConfig3{};
		const float x_data[11] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

		std::vector<float> m_Results;
		std::vector<float> m_Results2;
	};
}

