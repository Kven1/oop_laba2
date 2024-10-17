#include <vector>
#include <cassert>
#include <iostream>

enum class Class { Vane, Station };

class Vane {
protected:
    int _x;
    int _y;
    float _wind_speed;
    float _wind_direction;

public:
    virtual ~Vane() = default;

    Vane(
        const int x,
        const int y,
        const float wind_speed,
        const float wind_direction) : _x(x), _y(y), _wind_speed(wind_speed),
                                      _wind_direction(wind_direction) {
    };

    Vane(const Vane &vane) : Vane(vane._x, vane._y, vane._wind_speed,
                                  vane._wind_direction) {
    };

    Vane() : Vane(0, 0, 0, 0) {
    };

    [[nodiscard]] virtual Class what_class() const { return Class::Vane; };
    [[nodiscard]] int x() const { return _x; };
    [[nodiscard]] int y() const { return _y; };
    [[nodiscard]] float wind_speed() const { return _wind_speed; };
    [[nodiscard]] float wind_direction() const { return _wind_direction; };
};

class DetailedMetric {
    float _wind_speed;
    float _wind_direction;
    float _temperature;
    float _pressure;

public:
    DetailedMetric(const float wind_speed, const float wind_direction,
                   const float temperature,
                   const float pressure) : _wind_speed(wind_speed),
                                           _wind_direction(wind_direction),
                                           _temperature(temperature),
                                           _pressure(pressure) {
    }

    DetailedMetric(const DetailedMetric &metric) : DetailedMetric(
        metric.wind_speed(), metric.wind_direction(), metric.temperature(),
        metric.pressure()) {
    };

    DetailedMetric() : DetailedMetric(0, 0, 0, 0) {
    }

    [[nodiscard]] float wind_speed() const { return _wind_speed; };
    [[nodiscard]] float wind_direction() const { return _wind_direction; };
    [[nodiscard]] float temperature() const { return _temperature; };
    [[nodiscard]] float pressure() const { return _pressure; };
};

class Station final : public Vane {
    std::vector<DetailedMetric> _metrics_history;
    float _temperature;
    float _pressure;

public:
    Station(const std::vector<DetailedMetric> &metrics_history,
            const int x, const int y)
        : Vane(x, y, metrics_history.back().wind_speed(), metrics_history.back().wind_direction()),
          _temperature(metrics_history.back().temperature()),
          _pressure(metrics_history.back().pressure()) {
    };

    Station(const Station &station)
        : Station(station._metrics_history,
                  station._x, station._y) {
    };

    Station() : Station({{0, 0, 0, 0}}, 0, 0) {
    };

    [[nodiscard]] Class what_class() const override { return Class::Station; };

    [[nodiscard]] std::vector<DetailedMetric> metrics_history() const {
        return _metrics_history;
    }

    [[nodiscard]] float temperature() const { return _temperature; };
    [[nodiscard]] float pressure() const { return _pressure; };

    void add_new_metric(const DetailedMetric &metric) {
        _metrics_history.emplace_back(
            metric
        );
        _temperature = metric.temperature();
        _pressure = metric.pressure();
        _wind_speed = metric.wind_speed();
        _wind_direction = metric.wind_direction();
    }
};

int main() {
    // Testing Vane class
    Vane vane1(10, 20, 5.5, 90);
    assert(vane1.x() == 10);
    assert(vane1.y() == 20);
    assert(vane1.wind_speed() == 5.5f);
    assert(vane1.wind_direction() == 90.0f);
    assert(vane1.what_class() == Class::Vane);

    Vane vane2(vane1);
    assert(vane2.x() == 10);
    assert(vane2.y() == 20);
    assert(vane2.wind_speed() == 5.5f);
    assert(vane2.wind_direction() == 90.0f);

    // Testing DetailedMetric class
    DetailedMetric metric1(6.5, 180, 25.0, 1013.25);
    assert(metric1.wind_speed() == 6.5f);
    assert(metric1.wind_direction() == 180.0f);
    assert(metric1.temperature() == 25.0f);
    assert(metric1.pressure() == 1013.25f);

    DetailedMetric metric2(metric1);
    assert(metric2.wind_speed() == 6.5f);
    assert(metric2.wind_direction() == 180.0f);
    assert(metric2.temperature() == 25.0f);
    assert(metric2.pressure() == 1013.25f);

    // Testing Station class
    std::vector<DetailedMetric> metrics = {metric1, {7.0, 190, 26.0, 1012.0}};
    Station station(metrics, 30, 40);
    assert(station.x() == 30);
    assert(station.y() == 40);
    assert(station.wind_speed() == 7.0f);
    assert(station.wind_direction() == 190.0f);
    assert(station.temperature() == 26.0f);
    assert(station.pressure() == 1012.0f);
    assert(station.what_class() == Class::Station);

    // Adding new metric to Station
    DetailedMetric new_metric(8.0, 200, 27.0, 1011.5);
    station.add_new_metric(new_metric);
    assert(station.wind_speed() == 8.0f);
    assert(station.wind_direction() == 200.0f);
    assert(station.temperature() == 27.0f);
    assert(station.pressure() == 1011.5f);

    // Testing interaction between Vane and Station
    Vane* vane_ptr = &station;
    assert(vane_ptr->what_class() == Class::Station);
    assert(vane_ptr->wind_speed() == 8.0f);
    assert(vane_ptr->wind_direction() == 200.0f);
    assert(vane_ptr->x() == 30);
    assert(vane_ptr->y() == 40);

    // Testing interaction between Station and DetailedMetric
    const std::vector<DetailedMetric>& history = station.metrics_history();
    assert(history.size() == station.metrics_history().size());
    assert(history.back().temperature() == 27.0f);
    assert(history.back().pressure() == 1011.5f);

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}


