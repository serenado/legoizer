#include "lodepng/lodepng.h"
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <iterator>

using namespace std;

class Image
{
    public:
        std :: vector <unsigned char>  imgVec;
        unsigned width, height;
 
        //Constructor
        Image (std::vector <unsigned char> img, unsigned w, unsigned h)
        {
            imgVec = img;
            width = w;
            height = h;
        };
 
    unsigned get_index(unsigned row, unsigned col)
    {
      unsigned r = row<height?row:height;
      unsigned c = col<width?col:width;
 
      return (r * width + c) * 4;
    }

    std :: vector <int> get (int row, int col)
        {
            int index = (width * row + col) * 4;
            std :: vector <int> v = *(new std :: vector<int>());
            for (int i = 0; i < 3; i++)
            {
                v.push_back(imgVec[index + i]);
            }
             
            return v;
        }
};

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);
 
  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}


Image decodeOneStep(const char* filename) // Returns an instance of an image given the path to a png file
{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;
  //decode
  unsigned error = lodepng::decode(image, width, height, filename);
  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  Image img = *(new Image(image, width, height));
 
  return img;
}

std :: vector <unsigned char> average_cell (unsigned start_row, unsigned start_col, unsigned end_row, unsigned end_col, Image img)
{
 
  unsigned long red = 0;
  unsigned long green = 0;
  unsigned long blue = 0;
  float count = 0;
 
  for (unsigned row = start_row; row < end_row; row++)
  {
    for(unsigned col = start_col; col < end_col; col++)
    {
      int pix_index = img.get_index(row, col);
 
      red += (unsigned long) img.imgVec[pix_index];
      green += (unsigned long) img.imgVec[pix_index+1];
      blue += (unsigned long) img.imgVec[pix_index+2];
      count++;
    }
  }
 
  red = (unsigned char) ( (float)red/count );
  green = (unsigned char) ( (float)green/count );
  blue = (unsigned char) ( (float)blue/count );
 
  std::vector<unsigned char> v = *(new std::vector<unsigned char> ());
  v.push_back((unsigned char)red);
  v.push_back((unsigned char)green);
  v.push_back((unsigned char)blue);
  v.push_back((unsigned char)255);
 
  return v;
}

Image resize (Image original, float h_in)
{
 
  unsigned h_lego = (unsigned) (h_in * 25.4 / 8.0);
  unsigned w_lego = (unsigned) ( h_lego * (float)original.width/(float)original.height );
  unsigned lego_size = (unsigned)((float)original.height/(float)h_lego);
 
  std::vector<unsigned char> img_vec = *(new std::vector<unsigned char> ());
 
  for (unsigned i = 0; i < h_lego; i++)
  {
    for (unsigned j = 0; j < w_lego; j++)
    {
      std::vector<unsigned char> v = average_cell(i * lego_size, j * lego_size, (i+1) * lego_size, (j+1) * lego_size, original);
      for (auto &element: v)
      {
        img_vec.push_back(element);
      }
    }
  }
 
  Image image = *(new Image(img_vec, w_lego, h_lego));
 
  return image;
 
}
 
std::map <std::string, std::vector <int> > color_list()		//returns a map of Lego colors and RGB values
{
	std::map < std::string, vector<int> > colors;
	colors["white"] = {242, 243, 243};
	colors["grey"] = {161, 165, 162};
	colors["light yellow"] = {249,233, 153};
	colors["brick yellow"] = {215, 197, 153};
	colors["light green"] = {194, 218, 184};
	colors["light reddish violet"] = {232, 186, 199}; 
	colors["light orange brown"] = {203, 132, 66};
	colors["nougat"] = {204, 142, 104};
	colors["bright red"] = {196, 40, 27};
	colors["medium reddish violet"] = {196, 112, 160};
	colors["bright blue"] = {13, 105, 171};
	colors["bright yellow"] = {245, 205, 47};
	colors["earth_orange"] =  {98, 71, 50};
	colors["black"] = {27, 42, 42};
	colors["dark grey"] = {109, 110, 108};
	colors["dark green"] = {40, 127, 70};
	colors["medium green"] = {161, 196, 139};
	colors["light yellowish orange"] = {243, 207, 155};
	colors["bright green"] = {75, 151, 74};
	colors["dark orange"] = {160, 95, 52};
	colors["light bluish violet"] = { 193,202 ,222 };
	colors["light blue"] = { 180,210 ,227 };
	colors["light red"] = { 238, 196, 182};
	colors["medium red"] = {218 , 134, 121};
	colors["medium blue"] = {110 , 153,201 };
	colors["light grey"] = {199 ,193 ,183 };
	colors["bright violet"] = { 107, 50, 123};
	colors["bright yellowish orange"] = {226 ,155 ,63 };
	colors["bright orange"] = {218 , 133, 64};
	colors["bright bluish green"] = { 0, 143, 155};
	colors["earth yellow"] = {104, 92, 67};
	colors["bright bluish violet"] = {67, 84, 147};
	colors["medium bluish violet"] = {104, 116, 172};
	colors["medium yellowish green"] = {199, 210, 60};
	colors["medium bluish green"] = {85, 165, 175};
	colors["light bluish green"] = {183, 215, 213};
	colors["bright yellowish green"] = {164, 184, 70};
	colors["light yellowish green"] = {217, 228, 167};
	colors["medium yellowish orange"] = {231, 172, 88};
	colors["bright reddish orange"] = {211, 111, 76};
	colors["bright reddish violet"] = {146, 57, 120};
	colors["light orange"] = {234, 184, 145};
	colors["gold"] = {220, 188, 129};
	colors["dark nougat"] = {174, 122, 89};
	colors["silver"] = {156, 163, 168};
	colors["sand blue"] = {116, 134, 156};
	colors["sand violet"] = {135, 124, 144};
	colors["medium orange"] = {224, 152,100 };
	colors["sand yellow"] = {149, 138, 115};
	colors["earth blue"] = {32, 58, 86};
	colors["earth green"] = {39, 70, 44};
	colors["sand blue metallic"] = {121, 136, 161};
	colors["sand violet metallic"] = {149, 142, 163};
	colors["sand yellow metallic"] = {146, 135, 103};
	colors["dark grey metallic"] = {147, 136, 103};
	colors["black metallic"] = {22, 29, 50};
	colors["light grey metallic"] = {171, 173, 172};
	colors["sand green"] = {120, 144, 129};
	colors["sand red"] = {149, 121, 118};
	colors["dark red"] = {123, 46, 47};
	colors["gun metallic"] = {117, 108, 98};
	colors["curry"] = {215, 169, 75};
	colors["lemon metallic"] = {130, 138,93 };
	colors["fire yellow"] = {249, 214, 46};
	colors["flame yellowish orange"] = {232, 171, 45};
	colors["reddish brown"] = {105, 64, 39};
	colors["flame reddish orange"] = {207, 96, 36};
	colors["medium stone grey"] = {163, 162, 164};
	colors["royal blue"] = {70, 103, 164};
	colors["dark royal blue"] = {35, 71, 139};
	colors["bright reddish lilac"] = {142, 66, 133};
	colors["dark stone grey"] = {99,95 ,97 };
	colors["light stone grey"] = {229, 228, 222};
	colors["dark curry"] = {176, 142, 68};
	colors["faded green"] = {112, 149, 120};
	colors["turquoise"] = {121, 181,181 };
	colors["light royal blue"] = {159, 195, 233};
	colors["medium royal blue"] = {108, 129, 183};
	colors["rust"] = {143, 76, 42};
	colors["brown"] = {124, 92, 69};
	colors["reddish lilac"] = {150, 112, 159};
	colors["lilac"] = {107, 98, 155};
	colors["light lilac"] = {167, 169, 206};
	colors["bright purple"] = {205, 98, 152};
	colors["light purple"] = {228, 173, 200};
	colors["light pink"] = {220, 144, 149};
	colors["light brick yellow"] = {240, 213, 160};
	colors["warm yellowish orange"] = {235, 184, 127};
	colors["cool yellow"] = {253, 234, 140};
	colors["dove blue"] = {125, 187, 221};
	colors["medium lilac"] = {52, 43,117 };
	colors["transparent"] = {236, 236,236 };
	colors["transparent red"] = {205,236 ,236 };
	colors["transparent light blue"] = {193, 236,236 };
	colors["transparent blue"] = {123, 236, 236};
	colors["transparent yellow"] = {247, 241, 241};
	colors["transparent fluorescent reddish orange"] = {217, 133, 108};
	colors["transparent green"] = {132, 182, 141};
	colors["transparent fluorescent green"] = {248, 241, 141};
	colors["phosphorescent white"] = {236, 232, 222};
	colors["transparent brown"] = {191, 183, 177};
	colors["transparent medium reddish violet"] = {228, 173, 200};
	colors["transparent bright bluish violet"] = {165, 165, 203};
	colors["neon orange"] = {213, 115, 61};
	colors["neon green"] = {216, 221, 86};
	colors["transparent fluorescent blue"] = {207, 226, 247};
	colors["transparent fluorescent yellow"] = {255, 246,123 };
	colors["transparent fluorescent red"] = {225, 164, 194};
	colors["red flip/flop"] = {151, 105, 91};
	colors["yellow flip/flop"] = {180, 132, 85};
	colors["silver flip/flop"] = {137, 135, 136};

	return colors;
}

std::string colorize_pixel(std::vector<int> pixel)				//returns Lego color at speciic pixel
{
	std::map<string, std::vector <int> > colors = color_list();	//gets list of lego colors
	std::string closest_color = "";
	int difference = pow(255, 2) * 3 + 1;
	for (auto const& color : colors)			//loops through color list
	{
		int r1 = color.second[0];
		int r2 = pixel[0];

		int g1 = color.second[1];
		int g2 = pixel[1];

		int b1 = color.second[2];
		int b2 = pixel[2];

		if (sqrt ( pow(r1-r2, 2 ) + pow(g1-g2, 2) + pow(b1-b2, 2) ) < difference)		//distance formula for color
		{
			difference = sqrt ( pow(r1-r2, 2 ) + pow(g1-g2, 2) + pow(b1-b2, 2));
			closest_color = color.first;
		}
	}
	return closest_color;
}

void colorize_image(Image img)
{
	std::map<std::string, std::vector <int> > colors = color_list();
	std::vector<unsigned char> lego_vec;

	for (int row = 0; row < img.height; row++)
	{
		for (int col = 0; col < img.width; col++)
		{

			std::string color = colorize_pixel(img.get(row, col));
			lego_vec.push_back((unsigned char) (colors[color])[0]);
			lego_vec.push_back((unsigned char) (colors[color])[1]);
			lego_vec.push_back((unsigned char) (colors[color])[2]);
			lego_vec.push_back( (unsigned char) 255);		//alpha channel
		}
	}
	encodeOneStep("lego.png", lego_vec, img.width, img.height);
}

std::map <std::string, int> parts_list(Image img)			//returns instructions list for image
{
	//map <string, vector <int> > colors = color_list();
	std::map <std::string, int> instructions;
	for(int row = 0; row < img.height; row++)		//loop through each pixel of image
	{
		for(int col = 0; col < img.width; col++)
		{
			std::string lego_color = colorize_pixel(img.get(row, col));
			if (instructions.find(lego_color) == instructions.end())		//if lego color not in map yet
			{
				instructions[lego_color] = 1;
				//std::cout << "new color found: " << lego_color << std::endl;
			}
			else
			{
				instructions[lego_color] += 1;
				//std::cout << "adding to total of: " << lego_color << std::endl;
			}
		}
	}
	return instructions;
}

int main(int argc, char *argv[])
{
	
	const char* filename = argv[1];
	char* file;
	float height;

	//User input

	std::cout << "Enter height of mural in inches: ";
	std::cin >> height;

	//const char* filename = file;
	Image img = decodeOneStep(filename);			//create image
	
	Image new_image = resize(img, height);
	//const char* output_file = "resized.png";
	//encodeOneStep(output_file, new_image.imgVec, new_image.width, new_image.height);

//Output instrutions list
	std::map <std::string, int> instructions = parts_list(new_image);
	cout << "\n";
	for (auto const& entry : instructions)
	{
		std::cout << "You need " << entry.second << " pieces of " << entry.first << endl;
	}


	//Create new image with lego colors
	cout << "Creating new image file with Lego colors" << endl;
	colorize_image(new_image);
	cout << "New image created." << endl;
	


	return 0;

}