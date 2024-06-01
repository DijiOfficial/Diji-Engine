#include "ScoreBoard.h"
#include <fstream>

void pacman::ScoreBoard::Init()
{
    std::ifstream file{ "scoreboard.txt" };

    std::string line, text;
    while (std::getline(file, line, '\n'))
    {
        std::string part1 = line.substr(0, line.find(":"));
        std::string part2 = line.substr(line.find(":") + 1, line.size());

        m_ScoreboardMap.insert(std::pair<int, std::string>(std::stoi(part2), part1));
    }
}

void pacman::ScoreBoard::AddScore(const std::string& name, int score)
{
    const std::string fileName{ "scoreboard.txt" };
    std::ofstream file{ fileName, std::ios_base::app | std::ios_base::out };
    std::string text{ name + ":" + std::to_string(score) + "\n" };
    file << text;
    file.flush();
    file.close();

    m_ScoreboardMap.insert(std::pair<int, std::string>(score, name));
}

int pacman::ScoreBoard::GetHighestScore() const
{
    return m_ScoreboardMap.empty() ? 0 : m_ScoreboardMap.begin()->first;
}
