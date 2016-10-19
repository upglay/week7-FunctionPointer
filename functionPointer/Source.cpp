#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

const int width = 640;
const int height = 480;

float* pixels = new float[width * height * 3];

class GeometricObject
{
public:
	int a_;

	int center_x_, center_y_;

	int radius_;

	void draw()
	{
		draw_surrounding_object();
		
		drawLine(center_x_ - (radius_ / 2), center_y_ + (radius_ / 2), center_x_ + (radius_ / 2), center_y_ + (radius_ / 2));
		drawLine(center_x_ + (radius_ / 2), center_y_ + (radius_ / 2), center_x_ + (radius_ / 2), center_y_ - (radius_ / 2));
		drawLine(center_x_ + (radius_ / 2), center_y_ - (radius_ / 2), center_x_ - (radius_ / 2), center_y_ - (radius_ / 2));
		drawLine(center_x_ - (radius_ / 2), center_y_ - (radius_ / 2), center_x_ - (radius_ / 2), center_y_ + (radius_ / 2));
	}


	void init(const int& x, const int& y, const int& r)
	{
		center_x_ = x;
		center_y_ = y;
		radius_ = r;
	}

	void(GeometricObject::*draw_surrounding_object_callback)(void) = nullptr;

	void draw_surrounding_object()
	{
		(this->*draw_surrounding_object_callback)();
	}
	//virtual void draw(void) = 0;

	void drawCircle()
	{
		float thickness_ = 3;

		float theta = 2 * 3.1415926 / 1000.0f;
		float c = cosf(theta);//precalculate the sine and cosine
		float s = sinf(theta);
		float t;

		float x = radius_;
		float y = 0;

		for (int i = 0; i < thickness_; i++)
		{
			x = radius_ + i;
			y = 0;
			for (int ii = 0; ii < 1000; ii++)
			{
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 0] = 1.0f;	//red
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 1] = 0.0f;	//green
				pixels[((int)(x + center_x_) + width*(int)(y + center_y_)) * 3 + 2] = 0.0f;	//blue

																							//apply the rotation matrix
				t = x;
				x = c * x - s * y;
				y = s * t + c * y;
			}
		}
	}

	void drawBox()
	{
		drawLine(center_x_ - radius_, center_y_ + radius_, center_x_ + radius_, center_y_ + radius_);
		drawLine(center_x_ + radius_, center_y_ + radius_, center_x_ + radius_, center_y_ - radius_);
		drawLine(center_x_ + radius_, center_y_ - radius_, center_x_ - radius_, center_y_ - radius_);
		drawLine(center_x_ - radius_, center_y_ - radius_, center_x_ - radius_, center_y_ + radius_);
	}

	void drawLine(const int& i0, const int& j0, const int& i1, const int& j1)
	{
		if (i0 < i1)
		{
			for (int i = i0; i <= i1; i++)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				pixels[(i + width*j) * 3 + 0] = 1.0;	//red
				pixels[(i + width*j) * 3 + 1] = 0.0f;	//green
				pixels[(i + width*j) * 3 + 2] = 0.0f;	//blue
			}
		}
		else if (i0 > i1)
		{
			for (int i = i1; i <= i0; i++)
			{
				const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

				pixels[(i + width*j) * 3 + 0] = 1.0;	//red
				pixels[(i + width*j) * 3 + 1] = 0.0f;	//green
				pixels[(i + width*j) * 3 + 2] = 0.0f;	//blue
			}
		}
		else
		{
			if (j0 < j1)
			{
				for (int j = j0; j < j1; j++)
				{
					pixels[(i0 + width*j) * 3 + 0] = 1.0;	//red
					pixels[(i0 + width*j) * 3 + 1] = 0.0f;	//green
					pixels[(i0 + width*j) * 3 + 2] = 0.0f;	//blue
				}
			}
			else if (j0 > j1)
			{
				for (int j = j1; j < j0; j++)
				{
					pixels[(i0 + width*j) * 3 + 0] = 1.0f;	//red
					pixels[(i0 + width*j) * 3 + 1] = 0.0f;	//green
					pixels[(i0 + width*j) * 3 + 2] = 0.0f;	//blue
				}
			}
			else
			{
				pixels[(i0 + width*j0) * 3 + 0] = 1.0f;	//red
				pixels[(i0 + width*j0) * 3 + 1] = 0.0f;	//green
				pixels[(i0 + width*j0) * 3 + 2] = 0.0f;	//blue
			}
		}
	}

	static GeometricObject* getPointer(const std::string type_name)
	{
		auto new_ob = new GeometricObject;

		if (type_name == "Box")
		{
			new_ob->draw_surrounding_object_callback = &GeometricObject::drawBox;
		}
		else if (type_name == "Circle")
		{
			new_ob->draw_surrounding_object_callback = &GeometricObject::drawCircle;
		}
		else
		{
			new_ob = nullptr;
		}

		return new_ob;
	}
};

int main()
{

	GLFWwindow* window;


	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			pixels[(i + width*j) * 3 + 0] = 1.0f;	//red
			pixels[(i + width*j) * 3 + 1] = 1.0f;	//green
			pixels[(i + width*j) * 3 + 2] = 1.0f;	//blue
		}

	std::vector<GeometricObject*> geo_vector;
	geo_vector.push_back(GeometricObject::getPointer("Box"));
	geo_vector.push_back(GeometricObject::getPointer("Circle"));

	geo_vector[0]->init(100, 100, 50);
	geo_vector[1]->init(300, 100, 50);

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//geo_vector[0]->clearBackgroundColor(pixels);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//draw circle and square
		for (auto itr : geo_vector)
		{
			itr->draw_surrounding_object();
			//itr->draw();
		}

		glDrawPixels(640, 480, GL_RGB, GL_FLOAT, pixels);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}