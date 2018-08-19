#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

#define MAXFLOAT std::numeric_limits<float>::max()

vec3 color(const ray& r, hitable *world,int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, MAXFLOAT, rec))
	{
		ray scattered;
		vec3 attenuation;
		if(depth<50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else { return vec3(0, 0, 0); }
	}
	else
	{
		// Background Color
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
	}
}

hitable *random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertion(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for(int a = -11;a<11;++a)
	{
		for(int b = -11;b<11;++b)
		{
			float choose_mat = drand48();
			vec3 center(a + 0.9f*drand48(), 0.2f, b + 0.9f*drand48());
			if((center - vec3(4,0.2f,0)).length() > 0.9f)
			{
				if(choose_mat < 0.8f) // diffuse
				{
					list[i++] = new sphere(center, 0.2f, new lambertion(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if(choose_mat<0.95f) // metal
				{
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f*(1 + drand48()), 0.5f*(1 + drand48()), 0.5f*(1 + drand48()))));
				}
				else // glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new lambertion(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0f, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main()
{
	int nx = 1280; // ���ͼƬ���ȣ����أ�
	int ny = 720; // ���ͼƬ�߶ȣ����أ�
	int ns = 100; // ���ڿ���ݶ��ز����Ĵ���

	std::ofstream outfile("Result.ppm", std::ios_base::out); // ���������ļ���

	outfile << "P3\n" << nx << " " << ny << "\n255\n";	// 
	std::cout << "P3\n" << nx << " " << ny << "\n255\n"; // ����̨��ʾ������

	hitable *world = random_scene(); // ���ɳ����е�����

	vec3 lookfrom(13, 2, 3); // �������λ��
	vec3 lookat(0, 0, 0); // ���������λ��
	vec3 viewup(0, 1, 0); // ��������ϵķ���
	
	float dist_to_focus = 10.0;// (lookfrom - lookat).length();
	float apperture = 0.1f;
	float aspect = float(nx) / float(ny);

	camera cam(lookfrom,lookat, viewup,20, aspect,apperture,dist_to_focus);

	for (int j = ny - 1; j >= 0; --j)
	{
		for (int i = 0; i < nx; ++i)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; ++s)
			{
				float random = drand48();
				float u = float(i + random) / float(nx);
				float v = float(j + random) / float(ny);

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world,0);
			}
			col /= float(ns);

			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			
			outfile << ir << " " << ig << " " << ib << "\n";
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}