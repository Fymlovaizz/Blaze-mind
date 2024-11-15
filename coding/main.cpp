#include <SFML/Graphics.hpp>

#include <libraries.lib/algorithm.lib/QuadTree.hpp>
#include <libraries.lib/physic.lib/momentum.hpp>
#include <libraries.lib/math.lib/constant.hpp>
#include <libraries.lib/math.lib/random.hpp>
#include <libraries.lib/math.lib/vector.hpp>
#include <libraries.lib/math.lib/any.hpp>
#include <unordered_map>
#include <type_traits>
#include <utility>
#include <vector>

#include <NG_Tools/NG.hpp>

#include <evolution/entity.hpp>
#include <evolution/environment.hpp>

int main() {
    ng::Window window;

    ng::Bio program;
    window.append(&program);

    window.mainloop();
    std::cout << "The program has been closed in peace." << std::endl;
    return 0;
} 
