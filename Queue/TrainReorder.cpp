/*
 * @Date: 2024-10-15 19:33:58
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-21 15:09:52
 */
#include "Queue.hpp"
#include <vector>

int main() {
    int n, k, x;
    std::cin >> n >> k;

    CirQueue<int> input;
    for (int i = 0; i < n; ++i) {
        std::cin >> x;
        input.push(x);
    }

    std::vector<CirQueue<int>> buffers(k);
    CirQueue<int> output;
    int present = 1;

    while (output.size() < n) {
        while (true) {
            bool found = false;
            for (int i = 0; i < k; ++i) {
                if (!buffers[i]) continue;
                if (buffers[i].front() == present) {
                    output.push(buffers[i].pop());
                    ++present;
                    found = true;
                    break;
                }
            }
            if (!found) break;
        }
        while (input && input.front() != present) {
            int max_front = -1, max_index = -1;
            for (int i = 0; i < k; ++i) {
                if (!buffers[i]) continue;
                int bf = buffers[i].back();
                if (bf < input.front()) {
                    if(bf >= max_front) {
                        max_front = bf;
                        max_index = i;
                    }
                }
            }
            if (max_front == -1) {
                int index = 0;
                while (index < k) {
                    if (!buffers[index]) break;
                    ++index;
                }
                if (index >= k) {
                    std::cout << "Unsolvable.\n";
                    return 0;
                }
                buffers[index].push(input.pop());
            } else {
                buffers[max_index].push(input.pop());
            }
        }
        if (input) {
            output.push(input.pop());
            ++present;
        }

        std::cout << "Input: " << input << '\n';
        for (int i = 0; i < k; ++i) {
            std::cout << "Buffer No." << i + 1 << ": " << buffers[i] << '\n';
        }
        std::cout << "Output: " << output << '\n';
        std::cout << "---------------------\n";
    }

   std::cout << "Success. -> " << output ;

    return 0;
}
