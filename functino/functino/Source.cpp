#include <iostream>
#include <vector>

class Test
{
public:
	void(Test::*callbeck_)(void) = nullptr;

	void execute()
	{
		(this->*callbeck_)();
	}

	void draw()
	{
		std::cout << "item" << std::endl;
	}

	void drawCircle()
	{
		std::cout << "Circle" << std::endl;
	}

	void drawBox()
	{
		std::cout << "Box" << std::endl;
	}

	static Test* choose(std::string type)
	{
		auto new_one = new Test;

		if (type == "Circle")
		{
			new_one->callbeck_ = &Test::drawCircle;
		}
		else if (type == "Box")
		{
			new_one->callbeck_ = &Test::drawBox;
		}

		return new_one;
	}

};

int main()
{
	std::vector<Test*> my_vec;
	my_vec.push_back(Test::choose("Circle"));
	my_vec.push_back(Test::choose("Box"));

	for (auto itr : my_vec)
	{
		itr->execute();
		itr->draw();
	}

	return 0;
}