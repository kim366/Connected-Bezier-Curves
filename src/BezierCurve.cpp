#include "BezierCurve.hpp"
#include "Misc.hpp"
#include <cassert>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

BezierCurve::BezierCurve()
{
    std::fill(_points.begin(), _points.end(), sf::Vector2f{-1, -1});
}

void BezierCurve::makeCurve()
{
    if (fullyInitialized())
    {
        for (float t{0}; t <= 1.01; t += 1.f / NUM_SEGMENTS)
        {
            std::vector<sf::Vector2f> remaining_points;
            remaining_points.resize(4);
            std::copy(_points.begin(), _points.end(), remaining_points.begin());

            while (remaining_points.size() > 1)
            {
                std::vector<sf::Vector2f> current_remaining_points;
                for (auto point{remaining_points.cbegin()}; point != remaining_points.cend();)
                {
                    auto cur_point{point};
                    if (++point != remaining_points.cend())
                        current_remaining_points.push_back(vinterpolate(t, *cur_point, *point));
                }
                remaining_points = current_remaining_points;
            }

            if (remaining_points.size() == 1)
                _curve.push_back(remaining_points[0]);
        }
    }
}

void BezierCurve::setNextPoint(sf::Vector2f point_)
{
    auto found{std::find(_points.begin(), _points.end(), sf::Vector2f{-1, -1})};
    assert(found != _points.end());
    *found = point_;

    if (fullyInitialized())
        makeCurve();
}

bool BezierCurve::fullyInitialized() const
{
    return std::find(_points.begin(), _points.end(), sf::Vector2f{-1, -1}) == _points.end();
}

void BezierCurve::draw(sf::RenderTarget& target_, sf::RenderStates) const
{
    for (int point_idx{0}; point_idx < 4; ++point_idx)
    {
        sf::CircleShape circle{2};
        circle.setOrigin(2, 2);
        circle.setPosition(_points[point_idx]);
        circle.setFillColor(point_idx == 1 || point_idx == 2 ? sf::Color{100, 100, 100} : sf::Color{255, 255, 255});
        target_.draw(circle);
    }

    if (fullyInitialized())
        draw_lines(_curve, target_, sf::Color::Red);
}
