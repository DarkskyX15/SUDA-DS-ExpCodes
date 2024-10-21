/*
 * @Date: 2024-09-02 14:16:33
 * @Author: DarkskyX15
 * @LastEditTime: 2024-09-03 22:39:01
 */

#ifndef _LIFE_GAME_HPP_
#define _LIFE_GAME_HPP_

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

namespace std {
    template<>
    class hash<pair<int, int>> {
    private:
        template <typename T>
        static inline void hash_combine(size_t& seed, const T& val) {
            seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        template <typename T>
        static inline void hash_seed(size_t& seed, const T& final) {
            hash_combine(seed, final);
        }

        template <typename T, typename... Types>
        static inline void hash_seed(size_t& seed, const T& val, const Types& ... args) {
            hash_combine(seed, val);
            hash_seed(seed, args...);
        }

        template <typename... Types>
        static inline size_t get_hash(const Types& ... args) {
            size_t seed = 0;
            hash_seed(seed, args...);
            return seed;
        }
    public:
        size_t operator() (const pair<int, int>& obj) const noexcept {
            return get_hash(obj.first, obj.second);
        }
    };
}

namespace LGame {

    enum class cell_state: unsigned char {
        DIE_OUT, THRIVE, KEEP, DEAD
    };

    class Cell;
    class Spore;
    typedef std::unordered_set<const Cell*> cell_collection;
    typedef std::unordered_map<std::pair<int, int>, Spore*> spore_collection;

    class Spore {
    private:
        std::pair<int, int> position;
        const Cell* mother;
        short neighbour_count;
    public:
        Spore(int x, int y):
        position(x, y), neighbour_count(0) { mother = nullptr; }
        ~Spore() = default;

        Spore(const Spore& spore) = default;
        Spore& operator= (const Spore& spore) = default;

        const std::pair<int, int>* getPositionRef() const { return &position; }
        void addCount() { ++neighbour_count; }
        cell_state calcState() const {
            if (mother != nullptr) {
                if ( neighbour_count == 0 ||
                     neighbour_count == 1 ||
                     neighbour_count >= 4 )
                    return cell_state::DIE_OUT;
                if (neighbour_count == 2 || neighbour_count == 3) 
                    return cell_state::KEEP;
            } else {
                if (neighbour_count == 3) return cell_state::THRIVE;
                else return cell_state::DEAD;
            }
            return cell_state::DEAD;
        }
        int getCnt() const { return this->neighbour_count; }
        const Cell* getMother() const { return this->mother; }
        void setMother(const Cell* mother) { this->mother = mother; }
    };

    class Cell {
    private:
        void tryInsertSpore(spore_collection& spc, const std::pair<int, int>& pair, bool ism) const {
            auto iter = spc.find(pair);
            if (iter != spc.cend()) {
                if (!ism) iter->second->addCount();
                else iter->second->setMother(this);
            } else {
                Spore* new_spore = nullptr;
                if (!ism) { 
                    new_spore = new Spore(pair.first, pair.second);
                    new_spore->addCount();
                } else {
                    new_spore = new Spore(pair.first, pair.second);
                    new_spore->setMother(this);
                }
                spc.insert(std::make_pair(pair, new_spore));
            }
        }
    public:
        std::pair<int, int> position;
        Cell(int x, int y): position(x, y) {}
        void generateSporeList(spore_collection& sp_collect) const {
            tryInsertSpore(sp_collect, position, true);
            tryInsertSpore(sp_collect, {position.first - 1, position.second}, false);
            tryInsertSpore(sp_collect, {position.first + 1, position.second}, false);

            tryInsertSpore(sp_collect, {position.first - 1, position.second + 1}, false);
            tryInsertSpore(sp_collect, {position.first, position.second + 1}, false);
            tryInsertSpore(sp_collect, {position.first + 1, position.second + 1}, false);

            tryInsertSpore(sp_collect, {position.first - 1, position.second - 1}, false);
            tryInsertSpore(sp_collect, {position.first, position.second - 1}, false);
            tryInsertSpore(sp_collect, {position.first + 1, position.second - 1}, false);
        }
    };

    class Render {
    private:
        std::pair<int, int> position, size, grid_len;
        int max_size, void_size;
        
        inline bool checkDisplay(const std::pair<int, int>& pos) const {
            if ((abs(pos.first - position.first) <= grid_len.first) &&
                (abs(pos.second - position.second) <= grid_len.second)) {
                return true;
            }
            return false;
        }

    public:
        Render(std::pair<int, int> camera_pos, std::pair<int, int> view_size,
                int max_view_size):
        position(camera_pos), size(view_size), max_size(max_view_size) {
            void_size = (max_size - 1) / 2;
            grid_len = {(size.first - 1) / 2, (size.second - 1) / 2};
        }

        void operator() (const cell_collection* map) const {
            //clear
            std::cout << "\033[1;1f\033[0J";

            // render game
            std::string base("\033["), build;
            build = base + std::to_string(void_size + 1) + ';' +
                    std::to_string(void_size * 2 + 1) + 'f';
            std::cout << build << "><";
            for (auto ptr : *map) {
                if (checkDisplay(ptr->position)) {
                    int pt_pos_x = void_size + (ptr->position).first - position.first;
                    pt_pos_x = pt_pos_x * 2;
                    int pt_pos_y = void_size - (ptr->position).second + position.second;
                    build = base + std::to_string(pt_pos_y + 1) + ';' 
                            + std::to_string(pt_pos_x + 1) + 'f';
                    std::cout << build << "██";
                }
            }
        }
        
        void additionalInfo(size_t cc_size, size_t gen, size_t rt, size_t ct) const {
            // render info
            std::string left_side = std::to_string(void_size * 4 + 8);
            std::cout <<"\033[5;" << left_side << 'f'
                    << "center position: " << position.first 
                    << ' ' << position.second;
            std::cout <<"\033[6;" << left_side << 'f'
                    << "total cells: " << cc_size;
            std::cout <<"\033[7;" << left_side << 'f'
                    << "present generation: " << gen;
            std::cout <<"\033[8;" << left_side << 'f'
                    << "render cost(ms): " << rt / 1000;
            std::cout <<"\033[9;" << left_side << 'f'
                    << "frame cost(us): " << ct;
        }

        void saveScreen(const std::string& save_path, 
                        const cell_collection* map) const {
            std::unordered_set<std::pair<int, int>> points;
            for (auto cell: *map) {
                points.insert(cell->position);
            }
            std::ofstream pat_out(save_path, std::ios::out);
            if (!pat_out) return ;
            for ( int i = position.second + grid_len.second;
                  i >= position.second - grid_len.second; --i ) {
                for ( int j = position.first - grid_len.first; 
                      j <= position.first + grid_len.first; ++j) {
                    auto iter = points.find({j, i});
                    if (iter == points.cend()) pat_out << 'o';
                    else pat_out << 'x';
                }
                pat_out << '\n';
            }
            pat_out.close();
        }

        const std::pair<int, int>* getCameraPosition() const {
            return &(this->position);
        }

        void translate(int x, int y) {
            position.first += x;
            position.second += y;
        }
    };

    class LifeGame {
    private:
        cell_collection *frame;
        std::size_t generation;

        static void deleteFrame(const cell_collection* obj) {
            if (obj != nullptr) {
                for (auto ptr : *obj) { delete ptr; }
                delete obj;
                obj = nullptr;
            }
        }
    public:
        LifeGame(cell_collection* first_frame) {
            frame = first_frame;
            generation = 1;
        }

        const cell_collection* getFrameRef() const { return this->frame; }
        void UpdateFrame() {
            spore_collection* sp_collect = new spore_collection();
            for (auto cell : *frame) {
                cell->generateSporeList(*sp_collect);
            }
            for (auto pair : *sp_collect) {
                cell_state result = pair.second->calcState();
                if (result == cell_state::THRIVE) {
                    auto position = pair.second->getPositionRef();
                    auto new_cell = new Cell(position->first, position->second);
                    frame->insert(new_cell);
                } else if (result == cell_state::DIE_OUT) {
                    auto mother = pair.second->getMother();
                    frame->erase(mother);
                    delete mother;
                }
                delete pair.second;
            }
            delete sp_collect;
            ++generation;
        }
        const std::size_t getGeneration() const { return this->generation; }

        ~LifeGame() {
            deleteFrame(frame);
        }
    };

    void insertCellCollection(cell_collection& cc, const Cell* cell) {
        for (auto ptr : cc) {
            if (ptr->position.first == cell->position.first &&
                ptr->position.second == cell->position.second) {
                delete cell;
                return;
            }
        }
        cc.insert(cell);
    }

    void eraseCellAt(cell_collection& cc, const std::pair<int, int>* position) {
        for (auto ptr : cc) {
            if (ptr->position.first == position->first &&
                ptr->position.second == position->second) {
                cc.erase(ptr);
                delete ptr;
                break;
            }
        }
    }

} /* namespace LGame */

#endif /* _LIFE_GAME_HPP_ */
