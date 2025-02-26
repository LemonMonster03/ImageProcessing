#ifndef _IMG_H_
#define _IMG_H_

#include "opencv2/opencv.hpp"
#include "vector"
#include "string"
#include "map"
#include "queue"
#include "algorithm"
using namespace std;
using namespace cv;

#define TYPE string
#define FILE_NAME string

struct RGB {
    int R;
    int G;
    int B;

    RGB(int r=0,int g=0,int b=0){
        R=r;
        G=g;
        B=b;
    }

    bool operator==(const RGB& other) const {
        return R == other.R && G == other.G && B == other.B;
    }
};

struct cmp
{
    bool operator()(const RGB& rgb1, const RGB& rgb2) const
    {
        if (rgb1.R != rgb2.R) {
            return rgb1.R < rgb2.R;
        } 
        else if (rgb1.G != rgb2.G) {
            return rgb1.G < rgb2.G;
        } 
        else {
            return rgb1.B < rgb2.B;
        }
    }
};

template <>
struct std::hash<RGB> {
    std::size_t operator()(const RGB& rgb) const {
        std::size_t h1 = std::hash<int>{}(rgb.R);
        std::size_t h2 = std::hash<int>{}(rgb.G);
        std::size_t h3 = std::hash<int>{}(rgb.B);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct Trible{
    int rol;
    int col;
    RGB val;

    Trible(int r=0,int c=0,RGB v={0,0,0}):rol(r),col(c),val(v){}
};

struct ppm{
    int size;//元素个数
    int total_rol;//总行数
    int total_col;//总列数
    vector<Trible> data;//元素

    ppm(int size=0,int rol=0,int col=0){
        this->size=size;
        data.resize(size);
        total_col=col;
        total_rol=rol;
    }

    void init(int size=0,int rol=0,int col=0){
        this->size=size;
        data.resize(size);
        total_col=col;
        total_rol=rol;
    }
};

struct TreeNode{
	int pro;//出现频次 
	RGB data;//待编码的RGB组
	
	TreeNode* left;
	TreeNode* right;
	
	TreeNode(int _pro,RGB _data,TreeNode* _left=nullptr, TreeNode* _right=nullptr){
		pro=_pro;
		data=_data;
		left=_left;
		right=_right;
	}
	
};

struct cmpchar{
	bool operator()(TreeNode* a, TreeNode* b){
		return a->pro > b->pro;
	}
};


/*读取ppm格式的图片的矩阵*/
class Picture{
    public:
        Picture(){root=nullptr;}
        bool check(FILE_NAME name,int x);
        void read_img(FILE_NAME name);//读取图片
        void gray_img(FILE_NAME name);//转灰度图
        void resize_img(FILE_NAME name);//缩放图片

		bool getCount(FILE_NAME name);
		void BuildHuffmanTree();
		void getCodeTable();
		void dfs(TreeNode* root,string code);
		// void encode();
		void compress(FILE_NAME name);
		void decompress(FILE_NAME name);
		void delnode(TreeNode* &node);
        ~Picture(){
            delnode(root);
        }
        
    private:
        vector<RGB> rgb;
        
        ppm pixel_matrix;//矩阵
        TYPE format;//图片类型
        int width;//图片宽度
        int height;//图片高度
        int max_value;//像素最大值
        FILE_NAME pic_name;//文件名

        TreeNode* root;
		unordered_map<RGB,string> CodeTable;//哈夫曼编码表 1(rgb)
		map<RGB,int,cmp> FrequencyMap;//出现频次 1(rgb)
		int count;
};


#endif  //_IMG_H_
