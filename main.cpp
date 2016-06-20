#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <fstream>
#include <windows.h>
#include <time.h>



using namespace std;
//////////////////////////////////////////////////////////////小矩阵求逆//////////////////////////////////////////////////////////////////
void Inv(double **a,int n, int m)
{

	double b[3][3];
	double tmp=a[0][0]*a[1][1]*a[2][2]+a[0][1]*a[1][2]*a[2][0]+a[0][2]*a[1][0]*a[2][1]-
						a[0][2]*a[1][1]*a[2][0]-a[0][1]*a[1][0]*a[2][2]-a[0][0]*a[1][2]*a[2][1];

	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
	{
		b[i][j]=a[(i+1)%3][(j+1)%3]*a[(i+2)%3][(j+2)%3]-a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3];
		b[i][j]/=tmp;
	}

	for(int i2=0;i2<3;i2++)
		for(int j=0;j<3;j++)
	{
		a[i2][j]=b[i2][j];
	}

}

//////////////////////////////////////////////////////////////稀疏矩阵求逆//////////////////////////////////////////////////////////////////
void inv(double **a, int n, int h, int w)   //n的1.5次方
{
        time_t  start,endt;
        start=clock();

        double **b=new double*[n];//逆矩阵存储
        for(int i3=0;i3<n;i3++)            //初始化b为单位阵
        {
            b[i3]=new
            double[n];
            for(int j=0;j<n;j++)
                b[i3][j]=0;
        }
        for(int i4=0;i4<n;i4++)
            b[i4][i4]=1;

        int rs,cs,rrs;
        double mik;
        int all;

        int len=2*h+3;                                      //一个常用的长度
        int start_n,end_n;                              //循环起始和结束的值，为了减少计算次数


	for(all=0;all<=n;all++)                        //顺序高斯消去法化左下角为零
	{
     start_n=all+1;                                 //开始和结束的地方
     end_n=all+len<n-1?all+len:n-1;
        for(rs=start_n;rs<all+len;rs++)
        {

                if(rs>n-1)break;
                if(a[rs][all]==0)continue;
                mik=a[rs][all]/a[all][all];
                for(cs=start_n;cs<all+len;cs++)
            {
                if(cs>n-1)break;
                a[rs][cs]-=mik*a[all][cs];
            }
            for(int i5=0;i5<=all;i5++)
            {
                b[rs][i5]-=mik*b[all][i5];
            }
        }

	}




	double temp;
	for(all=0;all<n;all++)
	 {
	  temp=a[all][all];
	  end_n=all+len;
	  for(cs=all;cs<end_n&&cs<n;cs++)
	  {
	   a[all][cs]/=temp;
	  }
	  for(int cs=0;cs<=all;cs++)
		b[all][cs]/=temp;
	 }        //归一化






	 for(all=n-1;all>=0;all--)
	 {


		 start_n=all-1;
		 end_n=all-len;
		  for(rs=start_n;rs>end_n&&rs>=0;rs--)
		  {
				if(a[rs][all]==0)continue;
				mik=a[rs][all];
				for(int j=0;j<n;j++)
					b[rs][j]-=mik*b[all][j];
		  }


	 }        //逆序高斯消去法化增广矩阵左边为单位矩阵



//a=b;
for(int i=0;i<n;i++)
{
    for(int j=0;j<n;j++)
        a[i][j]=b[i][j];
        //为了和逆高斯消去的简化配合
        //a[i][j]=b[j][i];
}

endt=clock();
cout<<"总计  "<<double(endt-start)/CLOCKS_PER_SEC;
}


//////////////////////////////////////////////////////主函数////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
     ofstream fout1("111.txt");
     ofstream fout2("222.txt");
     ofstream fout3("333.txt");
     ofstream fout4("444.txt");
	 //载入文件
     IplImage *src = cvLoadImage("7.jpg");
      char* str = new char[20];
        CvScalar tmp;
        memset(str,0,20);
    if(!src)cout<<"fail!! "<<endl;
    int h=src->height;
    int w=src->width;
    int size=h*w;

    IplImage *dark_channel=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);//存储暗原色图片和最终结果
    IplImage *result=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,3);
    int win_size=7;
    //得到三个通道的浮点值
    double **dst_1=new double*[h];
    for(int i8=0;i8<h;i8++)
    {
        dst_1[i8]=new double[w];
        for(int j=0;j<w;j++)
            dst_1[i8][j]=(double)(int (cvGet2D(src,i8,j).val[2]))/255;
    }


    double **dst_2=new double*[h];
    for(int i6=0;i6<h;i6++)
    {
        dst_2[i6]=new double[w];
        for(int j=0;j<w;j++)
            dst_2[i6][j]=(double)(int (cvGet2D(src,i6,j).val[1]))/255;
    }

    double **dst_3=new double*[h];
    for(int i7=0;i7<h;i7++)
    {
        dst_3[i7]=new double[w];
        for(int j=0;j<w;j++)
            dst_3[i7][j]=(double)(int (cvGet2D(src,i7,j).val[0]))/255;
    }

        double **dark=new double*[h+1];
        for(int s=0;s<h+1;s++)
        {
            dark[s]=new double[w+1];
            for(int j=0;j<w+1;j++)
                dark[s][j]=1;//所有元素为1
        }

        double pp;
        double qq;
		//得出三个通道中的最小值
       for (int j1=0+win_size;j1<w-win_size;j1++)
            for (int i=win_size+0;i<h-win_size;i++)
        {
                pp=dst_1[i][j1]<dst_2[i][j1] ? dst_1[i][j1]:dst_2[i][j1];
                pp=pp<dst_3[i][j1] ? pp:dst_3[i][j1];
                    for (int n=j1-win_size;n<=j1+win_size;n++)
                        for(int m=i-win_size;m<=i+win_size;m++)
                        {
                            if(dark[m+1][n+1]>pp)
                            {
                                dark[m+1][n+1]=pp;
                            }
                        }
        }

    double *d_src=new double[h*w+1];
    uchar * ind=(uchar *)dark_channel->imageData;
    int p=1;
    for(int m=0;m<w;m++)
        for(int n=0;n<h;n++)
    {
        d_src[p++]=1-dark[n+1][m+1];
        ind++;
    }
    ////////////win_b的计算
    double *win_b=new double[size];
    for(int w=0;w<size;w++)
        win_b[w]=0;



    for(int i23=0;i23<h-1;i23++)
        for(int j=0;j<w-1;j++)
    {
        if(i23<7&&j<7)
        {
            win_b[(i23+1)*w+j]=d_src[(i23+1)*w+j+1];
        }

    }

//////////////////////////////////////////////////////////////L矩阵生成//////////////////////////////////////////////////////////////////
    double **win_var=new double *[3];
    for(int i34=0;i34<3;i34++)
        win_var[i34]=new double[3];


    int *row_inds=new int[size*81+1];
    int *col_inds=new int[size*81+1];
    double * vals=new double[size*81+1];
   int neb_size=9;
   win_size=1;
   double epsilon=0.0000001;
   int win_inds[10];

   int num=1;//col_inds,rol_inds,vals的长度记录
   int num_tmp;
 double winI[10][4];
    double win_mu[4];
   for(int j2=0+win_size;j2<w-win_size;j2++)
   {

       for(int i=0+win_size;i<h-win_size;i++)
       {
           int col2=j2*h+i;
           int col1=col2-h;
           int col3=col2+h;


           win_inds[1]=col1;
           win_inds[2]=col1+1;
           win_inds[3]=col1+2;
           win_inds[4]=col2;
           win_inds[5]=col2+1;
           win_inds[6]=col2+2;
           win_inds[7]=col3;
           win_inds[8]=col3+1;
           win_inds[9]=col3+2;

                winI[1][1]=  dst_1[i-1][j2-1];
                 winI[2][1]=dst_1[i][  j2-1];
                winI[3][1]=dst_1[i+1][j2-1];
                winI[4][1]=dst_1[i-1][ j2];
                winI[5][1]=dst_1[i][ j2];
                winI[6][1]=dst_1[i+1][j2];
                winI[7][1]=dst_1[i-1][ j2+1];
                winI[8][1]=dst_1[i][    j2+1];
                winI[9][1]=dst_1[i+1][j2+1];

                winI[1][2]=dst_2[i-1][j2-1];
                winI[2][2]=dst_2[i][    j2-1];
                winI[3][2]=dst_2[i+1][j2-1];
                winI[4][2]=dst_2[i-1][ j2];
                winI[5][2]=dst_2[i][ j2];
                winI[6][2]=dst_2[i+1][j2];
                winI[7][2]=dst_2[i-1][ j2+1];
                winI[8][2]=dst_2[i][    j2+1];
                winI[9][2]=dst_2[i+1][j2+1];

                 winI[1][3]=dst_3[i-1][j2-1];
                winI[2][3]=dst_3[i][    j2-1];
                winI[3][3]=dst_3[i+1][j2-1];
                winI[4][3]=dst_3[i-1][ j2];
                winI[5][3]=dst_3[i][ j2];
                winI[6][3]=dst_3[i+1][j2];
                winI[7][3]=dst_3[i-1][ j2+1];
                winI[8][3]=dst_3[i][    j2+1];
                winI[9][3]=dst_3[i+1][j2+1];

                //获得win_mu
                win_mu[1]=0;
                win_mu[2]=0;
                win_mu[3]=0;

                for(int z=1;z<10;z++)
                    win_mu[1]+=winI[z][1];
                win_mu[1]/=9;
                for(int c=1;c<10;c++)
                    win_mu[2]+=winI[c][2];
                win_mu[2]/=9;
                for(int v=1;v<10;v++)
                    win_mu[3]+=winI[v][3];
                win_mu[3]/=9;
                //获得win_var
                for(int n=0;n<3;n++)
                    for(int j=0;j<3;j++)
                        win_var[n][j]=0;

                for(int i12=0;i12<3;i12++)
                    for(int j=0;j<9;j++)
                    win_var[0][i12]+=winI[j+1][1]*winI[j+1][i12+1];

                for(int i0=0;i0<3;i0++)
                    for(int j=0;j<9;j++)
                     win_var[1][i0]+=winI[j+1][2]*winI[j+1][i0+1];

                for(int i14=0;i14<3;i14++)
                    for(int j=0;j<9;j++)
                    win_var[2][i14]+=winI[j+1][3]*winI[j+1][i14+1];

                for(int i15=0;i15<3;i15++)
                    for(int j=0;j<3;j++)
                        win_var[i15][j]/=9;

                for(int i16=0;i16<3;i16++)
                    for(int j=0;j<3;j++)
                            win_var[i16][j]-=win_mu[i16+1]*win_mu[j+1];

                win_var[0][0]+=epsilon/9;
                 win_var[1][1]+=epsilon/9;
                  win_var[2][2]+=epsilon/9;
//////////////////////////////////////////////////////////////小矩阵求逆//////////////////////////////////////////////////////////////////
                  Inv((double**)win_var,3,1);

                  //计算离差
                  for(int i=1;i<10;i++)
                    for(int j=1;j<4;j++)
                        winI[i][j]-=win_mu[j];
                //计算论文中的L
                    double tmp[10][4]={0};
                    double tvals[10][10]={0};//这里声明可以减少初始化
                    for(int i=1;i<10;i++)
                        for(int j=1;j<4;j++)
                    {
                        for(int k=0;k<3;k++)
                        tmp[i][j]+=winI[i][k+1]*win_var[k][j-1];
                        //tmp[i][j]++;
                    }

                    for(int i=1;i<10;i++)
                        for(int j=1;j<10;j++)
                            for(int k=1;k<4;k++)
                            tvals[i][j]+=tmp[i][k]*winI[j][k];

                    for(int i=1;i<10;i++)
                        for(int j=1;j<10;j++)
                        {tvals[i][j]++;
                        tvals[i][j]/=9;
                        }

                    //存储row_inds

                    num_tmp=num;

                    for(int m1=1;m1<10;m1++)
                        for(int n=1;n<10;n++)
                        {
                            row_inds[num++]=win_inds[n];
                            //cout<<row_inds[num-1]<<' ';
                        }

                    //存储col_inds
                    num=num_tmp;
                    for(int m2=1;m2<10;m2++)
                        for(int n=1;n<10;n++)
                        {
                            col_inds[num++]=win_inds[m2];
                        }


                    //存储vals
                    num=num_tmp;
                    for(int m=1;m<10;m++)
                        for(int n=1;n<10;n++)
                        {
                            vals[num++]=tvals[n][m];
                        }
       }
   }


//////////////////////////////////////////////////////////////L矩阵处理//////////////////////////////////////////////////////////////////


   double **A=new double*[size];
   for(int i45=0;i45<size;i45++)
   {
       A[i45]=new double [size];
       for(int j=0;j<size;j++)
        A[i45][j]=0;
   }
   for(int i56=1;i56<num;i56++)
   {
        A[row_inds[i56]-1][col_inds[i56]-1]-=vals[i56];//这里的行数和列数要转换！！
   }


        delete row_inds;
        delete col_inds;
        delete vals;

   //求总和
   double *sumA=new double[size];
   for(int i24=0;i24<size;i24++)
   {
       for(int k=0;k<size;k++)
            sumA[i24]+=A[i24][k];
   }



   //sumA中的元素放到对角线上,,,A的创建过程中已经有了减号
   for(int q=0;q<size;q++)
        A[q][q]-=(sumA[q]);
    for(int e=0;e<size;e++)
        A[e][e]+=win_b[e];

//    for(int i=0;i<size;i++)
//    {
//        for(int j=0;j<size;j++)
//        {
//            sprintf(str,"%.8f",A[i][j]);
//            fout1<<str<<' ';
//        }
//        fout1<<'\n';
//    }


/////////////////////////////////////////////////////////////大矩阵求逆//////////////////////////////////////////////////////////////////
        //G_S(A,size,h,w);//jacobi
        //Inv(A,size);//通用
        inv(A,size,h,w);//稀疏改进

    for(int i=0;i<size;i++)
        win_b[i]*=win_b[i];



    double tmp1;
    double *x=new double [size];

    for(int p1=0;p1<size;p1++)
    {
        tmp1=0;
        for(int j=0;j<size;j++)
        {
            tmp1+=A[p1][j]*win_b[j];
        }
        *(x+p1)=tmp1;
    }



    int o=0;
    for(int r=0;r<w;r++)
        for(int j=0;j<h;j++)
    {
        tmp.val[0]=int (x[o++]*255);
         cvSet2D(dark_channel,j,r,tmp);
    }

    cvNamedWindow("1",1);
    cvShowImage("1",dark_channel);
    cvSaveImage("dark.jpg",dark_channel);
    cvWaitKey(0);
    //去雾部分
    //for(int i=0;i<3;i++)
    int ppp[3];
    o=0;
        for(int j=0;j<w;j++)
            for(int k=0;k<h;k++)
    {
        ppp[0]=(int )(((dst_1[k][j]-0.8627)/x[o]+0.8627)*255);
        ppp[1]=(int )(((dst_2[k][j]-0.8627)/x[o]+0.8627)*255);
        ppp[2]=(int )(((dst_3[k][j]-0.8627)/x[o++]+0.8627)*255);

         tmp.val[0]=ppp[2];
         tmp.val[1]=ppp[1];
         tmp.val[2]=ppp[0];
         cvSet2D(result,k,j,tmp);
    }
    cvNamedWindow("2",1);
    cvShowImage("2",result);
    cvSaveImage("result.jpg",result);
    cvWaitKey(0);

    return 0;
}
