#include "GPIO.h"

GPIO::GPIO(int gpio, const GPIO::Direction& direction)
    :
    gpio_(gpio),
    direction_(direction),
    is_open_(false)
{
    if (gpio_ != -1) {
        is_open_ = setup();
        if (!is_open_) {
            throw GPIO::exception("Cannot initialize GPIO: " + std::to_string(gpio_));
        }
        set_direction(direction_);
    }
}

bool GPIO::setup()
{
    clean();
    FILE* file = fopen(std::string("/sys/class/gpio/export").c_str(), "w");
    if (!file) {
        return false;
    }
    fprintf(file, "%d", gpio_);
    fclose(file);
    return true;
}

bool GPIO::clean()
{
    if (!is_open_) {
        return true;
    }

    FILE* file = fopen(std::string("/sys/class/gpio/unexport").c_str(), "w");
    if (!file) {
        return false;
    }
    fprintf(file, "%d", gpio_);
    fclose(file);
    is_open_ = false;
    return true;
}

GPIO::~GPIO()
{
    clean();
}

void GPIO::set_direction(const GPIO::Direction& direction)
{
    if (!is_open_) {
        throw GPIO::exception("GPIO " + std::to_string(gpio_) + " is not initialized thus cannot set direction");
    }

    FILE* file = fopen(std::string("/sys/class/gpio/gpio" + std::to_string(gpio_) + "/direction").c_str(), "w");

    if (!file) {
        throw GPIO::exception("Unable to set direction of GPIO: " + std::to_string(gpio_));
    }

    direction_ = direction;

    if (direction == GPIO::Direction::In) {
        fprintf(file, "in");
    } else if (direction == GPIO::Direction::Out) {
        fprintf(file, "out");
    }

    fclose(file);
}

const GPIO::Direction& GPIO::get_direction() const noexcept
{
    return direction_;
}

void GPIO::set_value(int value)
{
    if (!is_open_) {
        throw GPIO::exception("GPIO " + std::to_string(gpio_) + " is not initialized thus cannot set value");
    }

    FILE* file = fopen(std::string("/sys/class/gpio/gpio" + std::to_string(gpio_) + "/value").c_str(), "w");
    if (!file) {
        throw GPIO::exception("Unable to set value of GPIO: " + std::to_string(gpio_));
    }

    fprintf(file, "%d", value);
    fclose(file);
}

int GPIO::get_value() {

    if(!is_open_) {
        throw GPIO::exception("GPIO " + std::to_string(gpio_) + " is not initialized thus cannot get value");
    }

    FILE* file = fopen(std::string("/sys/class/gpio/gpio" + std::to_string(gpio_) + "/value").c_str(), "r");
    if (!file) {
        throw GPIO::exception("Unable to get value of GPIO: " + std::to_string(gpio_));
    }

    int value = 0;
    int read_arguments = fscanf(file, "%d", &value);

    if (read_arguments != 1) {
        throw GPIO::exception("Unable to get value of GPIO: " + std::to_string(gpio_));
    }

    if (value != 0) {
        value = 1;
    }

    fclose(file);

    return value;
}

int GPIO::get_gpio_num() const noexcept
{
    return gpio_;
}

void GPIO::set_gpio_num(int gpio) {

    gpio_ = gpio;
    is_open_ = setup();

    if (!is_open_) {
        throw GPIO::exception("Cannot initialize GPIO: " + std::to_string(gpio_));
    }

    set_direction(direction_);

}

void GPIO::reset(int gpio, const GPIO::Direction& direction)
{
    gpio_ = gpio;
    if (gpio_ != -1) {
        clean();
        is_open_ = setup();
        if (!is_open_) {
            throw GPIO::exception("Cannot initialize GPIO: " + std::to_string(gpio_));
        }
        set_direction(direction);
    }
}


GPIO::exception::exception(const std::string& msg)
    :
    msg_(msg)
{
}

const char* GPIO::exception::what() const noexcept
{
    return msg_.c_str();
}