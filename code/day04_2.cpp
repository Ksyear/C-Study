#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    int N = 0, W = 0;
    std::cout << "Enter N and W: ";
    std::cin >> N >> W;

    if (W <= 0 || N <= 0 || W > N) {
        std::cerr << "[ERROR] invalid N or W\n";
        return 1;
    }

    std::vector<double> data;
    data.reserve(N);
    std::cout << "Enter " << N << " doubles:\n";
    for (int i = 0; i < N; ++i) {
        double x = 0.0;
        std::cin >> x;
        data.push_back(x);
    }

    std::cout << "Moving averages (W=" << W << "):";
    std::cout << std::fixed << std::setprecision(2);

    double window_sum = 0.0;
    for (int i = 0; i < W; ++i) {
        window_sum += data[i];
    }
    std::cout << ' ' << (window_sum / W);

    for (int i = W; i < N; ++i) {
        window_sum += data[i] - data[i - W];
        std::cout << ' ' << (window_sum / W);
    }
    std::cout << '\n';
    return 0;
}
