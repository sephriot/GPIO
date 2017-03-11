#ifndef GPIO_H
#define GPIO_H

#include <exception>
#include <string>

class GPIO
{
public:

    enum class Direction {
    	In,
    	Out
    };

    GPIO(int gpio = -1, const GPIO::Direction& direction = Direction::Out);
    ~GPIO();

    void set_direction(const GPIO::Direction& direction);
    const Direction& get_direction() const noexcept;
    void set_value(int value);
    int get_value();
    int get_gpio_num() const noexcept;
    void set_gpio_num(int gpio);

    class exception : public std::exception {
    public:
    	exception(const std::string& msg);
    	virtual ~exception() = default;
    	virtual const char* what() const noexcept;
    private:
    	std::string msg_;
    };


private:
	bool setup();
	bool clean();

    int gpio_;
    Direction direction_;
    bool is_open_;
};

#endif