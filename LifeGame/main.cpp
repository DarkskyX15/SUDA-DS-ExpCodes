/*
 * @Date: 2024-09-02 14:16:11
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-03 22:44:25
 */
# include "LifeGame.hpp"
# include "Timer.hpp"
# include <conio.h>
# include <windows.h>

# define CAMERA_SIZE 41

using namespace LGame;

int main(int argc, char* argv[]) {
    // change code page
    SetConsoleOutputCP(65001);
    // hide cursor
    std::cout << "\033[?25l";

    cell_collection* ptr = new cell_collection();

    LifeGame game(ptr);
    Timer::Timer<Timer::us> timer;
    Render render({0, 0}, {CAMERA_SIZE, CAMERA_SIZE}, CAMERA_SIZE);
    size_t last_fresh_time = 0;
    char c = 0;

    // show info
    std::cout << "使用W、A、S、D控制视野，按 空格 进入下一世代，\n"
            "按 I 用坐标添加细胞，按 O 在中心光标处放置细胞，按 P 删除光标处细胞，\n"
            "按 F 保存视野，按 L 加载图案，按 Esc 退出。\n\n按任意键继续...";
    getch();

    std::string r_sign("r"), c_sign("c");
    while (c != '\033') {
        timer.start(r_sign);
        render(game.getFrameRef());
        render.additionalInfo(
            ptr->size(), game.getGeneration(),
            timer.end(r_sign), last_fresh_time
        );
        c = getch();
        if (c == 'w') render.translate(0, 1);
        else if (c == 'a') render.translate(-1, 0);
        else if (c == 's') render.translate(0, -1);
        else if (c == 'd') render.translate(1, 0);
        else if (c == 'l') {
            std::cout << "\033[1;1f\033[0J\033[?25h"
                    "指定加载的图案路径:\n>>> ";
            auto present_pos = render.getCameraPosition();
            std::string pattern_path, line;
            std::getline(std::cin, pattern_path);
            if (!pattern_path.size()) continue;
            std::ifstream pat_in(pattern_path, std::ios::in);
            if (!pat_in) continue;
            int pat_y = 0;
            while (std::getline(pat_in, line)) {
                if (!line.size()) continue;
                for (int x = 0; x < line.size(); ++x) {
                    if (line[x] == 'x') {
                        auto cell_ptr = new Cell(
                            present_pos->first + x, 
                            present_pos->second + pat_y
                        );
                        insertCellCollection(*ptr, cell_ptr);
                    }
                }
                --pat_y;
            }
            pat_in.close();
            std::cout << "\033[?25l";
        } else if (c == 'f') {
            std::cout << "\033[1;1f\033[0J\033[?25h"
                    "指定保存路径:\n>>> ";
            std::string save_path;
            std::getline(std::cin, save_path);
            if (!save_path.size()) {
                save_path = "frame_" + std::to_string(game.getGeneration()) + ".txt";
            }
            render.saveScreen(save_path, ptr);
            std::cout << "\033[?25l";
        } else if (c == 'i') {
            int pos_x, pos_y;
            std::cout << "\033[1;1f\033[0J\033[?25h"
                        "输入空格分隔的坐标以插入点:\n>>> ";
            std::cin >> pos_x >> pos_y;
            auto cell_ptr = new Cell(pos_x, pos_y);
            insertCellCollection(*ptr, cell_ptr);
            std::cout << "\033[?25l";
        } else if (c == 'o') {
            auto cam_pos = render.getCameraPosition();
            auto cell_ptr = new Cell(cam_pos->first, cam_pos->second);
            insertCellCollection(*ptr, cell_ptr);
        } else if (c == 'p') {
            auto cam_pos = render.getCameraPosition();
            eraseCellAt(*ptr, cam_pos);
        } else if (c == ' ') {
            timer.start(c_sign);
            game.UpdateFrame();
            last_fresh_time = timer.end(c_sign);
        }
    }

    return 0;
}
