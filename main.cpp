#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <unordered_set>
#include <clocale>
#include "HashFuncGen.h"
#include "HyperLogLog.h"
#include "RandomStreamGen.h"


const int B_BITS = 12;
const int TOTAL_ITEMS = 100000;
const int CHECK_STEP = 1000;
const int NUM_RUNS = 10;
int main() {
    std::setlocale(LC_ALL, "");

    std::cout << "Запуск эксперимента HyperLogLog" << '\n';
    std::cout << "Биты (b): " << B_BITS << ", Всего элементов: " << TOTAL_ITEMS << ", Прогонов: " << NUM_RUNS << '\n';

    std::ofstream outFile("results.csv");
    outFile << "\xEF\xBB\xBF";

    outFile << "Шаг,Точное_Среднее,Оценка_Среднее,Сигма,Нижняя_Граница,Верхняя_Граница\n";

    int num_points = TOTAL_ITEMS / CHECK_STEP;

    std::vector<double> sum_estimates(num_points, 0.0);
    std::vector<double> sum_sq_estimates(num_points, 0.0);
    std::vector<double> sum_exact(num_points, 0.0);

    for (int run = 0; run < NUM_RUNS; ++run) {
        std::cout << "Обработка прогона " << run + 1 << "/" << NUM_RUNS << "..." << '\n';
        RandomStreamGen streamGen(run + 1);
        HyperLogLog hll(B_BITS, run + 1);
        std::unordered_set<std::string> exactSet;

        int point_index = 0;
        for (int t = 1; t <= TOTAL_ITEMS; ++t) {
            std::string item = streamGen.next();

            hll.add(item);
            exactSet.insert(item);
            if (t % CHECK_STEP == 0) {
                double est = hll.count();
                double exact = static_cast<double>(exactSet.size());

                sum_estimates[point_index] += est;
                sum_sq_estimates[point_index] += (est * est);
                sum_exact[point_index] += exact;

                point_index++;
            }
        }
    }

    std::cout << "Подсчет статистики и запись в файл" << '\n';

    for (int i = 0; i < num_points; ++i) {
        int current_step = (i + 1) * CHECK_STEP;

        double mean_est = sum_estimates[i] / NUM_RUNS;
        double mean_exact = sum_exact[i] / NUM_RUNS;

        double mean_sq = sum_sq_estimates[i] / NUM_RUNS;
        double variance = mean_sq - (mean_est * mean_est);
        if (variance < 0) variance = 0;
        double sigma = std::sqrt(variance);

        double lower = mean_est - sigma;
        double upper = mean_est + sigma;

        outFile << current_step << ","
                << mean_exact << ","
                << mean_est << ","
                << sigma << ","
                << lower << ","
                << upper << "\n";
    }

    outFile.close();
    std::cout << "Результаты сохранены в 'results.csv'" << '\n';

    return 0;
}
