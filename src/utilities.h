#include "vector2D.h"
#include <vector>
#include <cmath>
#include <iterator>


namespace utilities {
    class Triangle {
        public:
            Triangle() { }
            Triangle(CS248::Vector2D p0, CS248::Vector2D p1, CS248::Vector2D p2) {
                points[0] = p0;
                points[1] = p1;
                points[2] = p2;
            }

            struct TriangleRange {
                CS248::Vector2D from;
                CS248::Vector2D to;
            };

            TriangleRange get_range() {
                return {
                    .from=CS248::Vector2D(
                        std::floor(std::min(std::min(points[0].x, points[1].x), points[2].x)),
                        std::floor(std::min(std::min(points[0].y, points[1].y), points[2].y))),
                    .to=CS248::Vector2D(
                        std::ceil(std::max(std::max(points[0].x, points[1].x), points[2].x)),
                        std::ceil(std::max(std::max(points[0].y, points[1].y), points[2].y))),
                };
            }

            bool is_inside(CS248::Vector2D target) {
                bool is_facing = this->is_facing();
                CS248::Vector2D * tri_points[3] = { &points[0], is_facing ? &points[1] : &points[2], is_facing ? &points[2] : &points[1]  };
                for (int i = 0; i < 3; i++) {
                    auto E = *tri_points[(i + 1) % 3] - *tri_points[i];
                    auto V = target - *tri_points[i];
                    if (!(cross(V, E) <= 0)) return false;
                }
                return true;
            }

            bool is_facing() {
                auto AB = points[1] - points[0];
                auto AC = points[2] - points[0];
                return cross(AB, AC) >= 0;
            }

        private:
            CS248::Vector2D points[3];
    };
}