

#include <vector>
#include "QROBOT_COMM_H.hpp" 
int main()
{
/*	std::vector <std::vector<int> > vec2D(5, std::vector<int>(4, 1));
 
	for(auto vec : vec2D)
	{
		for(auto x : vec)
			std::cout<<x<<" , ";
 
		std::cout << std::endl;
	}
 
	std::cout << std::endl;
 
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			vec2D[i][j] = i*j;
 
	for(auto vec : vec2D)
	{
		for(auto x : vec)
			std::cout<<x<<" , ";
 
		std::cout << std::endl;
	}
 
 
	vec2D.push_back(std::vector<int>(4, 11));
 
	std::cout << std::endl;
 
	for(auto vec : vec2D)
	{
		for(auto x : vec)
			std::cout<<x<<" , ";
 
		std::cout << std::endl;
	}

*/


       


	
	std::vector <std::vector<int> > vec2D_1;

       	for(int i = 0; i < 5; i++)
	for(int j = 0; j < 5; j++)
		//vec2D_1[i][j] = i*j;


	vec2D_1.push_back(std::vector<int>(4, 11));
//	vec2D_1.push_back(std::vector<int>(5, 10));



	std::vector <std::vector<int> > vec2D_2;


	 vec2D_2=vec2D_1;



	for(auto vec : vec2D_2)
        {
                for(auto x : vec)
                        std::cout<<x<<" :, ";

                std::cout << std::endl;
        }



         	
//vec2D.push_back(std::vector<int>(4, 11));









	return 0;
}
