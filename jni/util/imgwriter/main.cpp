/*****************************************************************************************
* Copyright (c) 2007 Hewlett-Packard Development Company, L.P.
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
* OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*****************************************************************************************/

/************************************************************************
 * SVN MACROS
 *
 * $LastChangedDate: 2011-08-23 13:34:03 +0530 (Tue, 23 Aug 2011) $
 * $Revision: 870 $
 * $Author: jitender $
 *
 ************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "LTKImageWriter.h"
#include "LTKStringUtil.h"
#include "Inc.h"

#define SUPPORTED_MIN_VERSION   "3.0.0"
#define CURRENT_VERSION   "4.0.0"


void startUp(string strRootDir,string strTempDir){

     string strPermission("chmod 777 ");

    makedir(strTempDir.c_str());

    DIR *rootDIR;
    DIR *tempDIR;
    struct dirent *tempDIREntry;

    rootDIR=opendir(strRootDir.c_str());
    if(rootDIR==NULL)
    {
         makedir(strRootDir.c_str());
#ifdef _WIN32
#else
          strPermission += strRootDir;
          system(strPermission.c_str());
#endif
          makedir(strTempDir.c_str());
    }
    else
    {
         tempDIR=opendir(strTempDir.c_str());
         if(tempDIR==NULL)
         {
              makedir(strTempDir.c_str());
         }
         closedir (tempDIR);
    }

#ifdef _WIN32
#else
     strPermission = "chmod 777 ";
     strPermission += strTempDir;
     system(strPermission.c_str());
#endif

     tempDIR=opendir(strTempDir.c_str());

      if(tempDIR == 0)
      {
          cout << "Permission denied (or) directory not found" << endl;
          return;
      }

     readdir(tempDIR);
     readdir(tempDIR);

     while((tempDIREntry=readdir(tempDIR))!=NULL)
     {

          string eachFile(tempDIREntry->d_name);
          string eachFilePath=strTempDir+"/"+eachFile;
          remove(eachFilePath.c_str());
     }
     closedir (tempDIR);
     closedir (rootDIR);

}

void writeImageFile(string srcFileName, string destFileName,
                      string dataFormat, string view,
                      unsigned char red, unsigned char green,
                      unsigned blue, int width, int height,
                      int size, bool bb, bool sp)
{
     try
     {
           cout<<"Creating image "<<destFileName<<" for "<<srcFileName<<endl;

           LTKImageWriter iw;
           iw.setColor(red,green,blue);

           int offset=0;
           if(view=="original")
           {
                offset=0;
           }
           else if(view=="strokewise")
           {
                offset=1;
           }

           iw.setOffset(offset);

           iw.showStartingPoint(sp);

           if(dataFormat=="unipen")
           {
                if(bb)
                {
                     if(size==0)
                     {
                          cout<<"size of dim cannot be zero while drawing the image with bounding box"<<endl;
                          exit(1);
                     }

                          iw.drawUnipenFileToImageWithBB(srcFileName.c_str(),destFileName.c_str(),size);
                }
                else
                {
                     if(width==0 && height==0 && size==0)
                     {
                               iw.drawUnipenFileToImage(srcFileName.c_str(),destFileName.c_str());
                     }
                     else if(width==0 && height==0)
                     {
                          iw.drawUnipenFileToImage(srcFileName.c_str(),destFileName.c_str(),size);
                     }
                     else
                     {
                          iw.drawUnipenFileToImage(srcFileName.c_str(),destFileName.c_str(),width,height);
                     }
                }
           }
           else if(dataFormat=="hpl")
           {
                if(width==0 && height==0 && size==0)
                {
                     iw.drawRawInkFileToImage(srcFileName.c_str(),destFileName.c_str());
                }
                else if(width==0 && height==0)
                {
                     iw.drawRawInkFileToImage(srcFileName.c_str(),destFileName.c_str(),size);
                }
                else
                {
                     iw.drawRawInkFileToImage(srcFileName.c_str(),destFileName.c_str(),width,height);
                }
           }
     }
     catch(...)
     {
          cout<<"Unable to generate image for the file:"<<srcFileName<<endl;
     }
}


void displayHelp()
{
     cout<<endl;
     cout<<"   -----------------------------------------------------"<<endl
         <<"   | Options |             Values                      |"<<endl
         <<"   |----------------------------------------------------"<<endl
         <<"   | -data   | unipen OR hpl default=unipen            |"<<endl
         <<"   | -view   | original OR strokewise default=original |"<<endl
         <<"   | -show   | on OR off default=on when set,displays  |"<<endl
         <<"   |         | in the browser                          |"<<endl
         <<"   | -file   | path to the file containing list of ink |"<<endl
         <<"   |         | files paths                             |"<<endl
         <<"   | -fext   | extension of files for which images need|"<<endl
         <<"   |         | to be generated default=.txt            |"<<endl
         <<"   | -color  | red,green,blue (each ranging from 0 to  |"<<endl
         <<"   |         | 255) default=0,0,0                      |"<<endl
         <<"   | -dim    | width,height OR size (size retains the  |"<<endl
         <<"   |         | aspect ratio) default=50,50             |"<<endl
         <<"   |  -bb    | true,false default=false when set,shows |"<<endl
         <<"   |         | the bounding box                        |"<<endl
         <<"   |  -sp    | true,false default=false when set,shows |"<<endl
         <<"   |         | the starting point of each stroke       |"<<endl
         <<"   -----------------------------------------------------"<<endl
         <<"1. The last 2 arguments are src and dest dir/file name. "<<endl
         <<"2. If show is on, mention only the src dir/file name."<<endl
         <<"3. If -file is set, images are generated in respective source directories."<<endl
         <<"4. The -fext option is applicable only when source directory is mentioned."<<endl
         <<"5. To write ink files as images, mention src & dest dir/file name."<<endl
         <<"6. The extension of the destination image file should be \"bmp\"."<<endl<<endl
         <<"Example:>> imgwriter -fext .new -view strokewise -color 0,255,255"<<endl
           <<" -dim 100 -sp true c:/data/usr2"<<endl;

}

string getFileName(string filePath)
{

      int forwardSlashIndex=filePath.find_last_of('/');
      int backwardSlashIndex=filePath.find_last_of('\\');
      int slashIndex;
      if(forwardSlashIndex>backwardSlashIndex && forwardSlashIndex!=string::npos)
      {
           slashIndex=forwardSlashIndex;
           slashIndex++;
      }
      else if(backwardSlashIndex!=string::npos)
      {
           slashIndex=backwardSlashIndex;
           slashIndex++;
      }
      else{
           slashIndex=0;
      }

      int dotIndex=filePath.find_last_of('.');
      if(dotIndex==string::npos)
      {
           dotIndex=filePath.length();
      }


      string fileName=filePath.substr(slashIndex,(filePath.length()-slashIndex+1)-(filePath.length()-dotIndex+1));

      return fileName;
}



int main(int argc,char *argv[]){
     try{

          string view="original"; //other value is strokewise
          int colorR=0;
          int colorG=0;
          int colorB=0;
          bool show=true;
          int dimW=50;
          int dimH=50;
          int size=0;
          string data="unipen"; //other value is hpl
          bool bb=false; //bounding box
          bool sp=false; //starting point
          string src;
          string dest;
          string filesList;
          string fileExt=".txt";

          string existingTempDir(TEMPDIR);

#ifdef _WIN32
#else
          existingTempDir = getenv(LIPIROOT_ENV_STRING) ;

          if(existingTempDir.empty())
               existingTempDir = getenv("ROOT") ;

          existingTempDir += "/bin/tmp";
          makedir(existingTempDir.c_str());

          string strPermission = "chmod 777 ";
          strPermission += existingTempDir;
          system(strPermission.c_str());
#endif

          string strRootDir=existingTempDir+"/marchtool";
          string strTempDir=strRootDir+"/"+"mt_tmp";

          if(argc==1)
          {
               displayHelp();
               exit(1);
          }

          for(int i=1;i<argc;)
        {
            string arg(argv[i]);
            if(arg=="-help")
            {
                 displayHelp();
                 exit(1);
            }
            else if(arg=="-view")
            {
                 i++;
                 string value(argv[i]);

                 if((value!="original" && value!="strokewise"))
                 {
                      cout<<"'"<<value<<"' not defined for view"<<endl;
                      cout<<"type imgwriter -help for options and their values"<<endl;
                      exit(1);
                 }
                 else
                 {

                      view=value;
                      i++;
                 }
            }
               else if(arg=="-color")
               {
                           i++;
                           string value(argv[i]);
                           vector<string> rgbVec;
                           LTKStringUtil::tokenizeString(value, ",", rgbVec);
                           if(rgbVec.size()!=3)
                           {
                                 cout<<"'"<<value<<"' not defined for color"<<endl;
                                 cout<<"type imgwriter -help for options and their values"<<endl;
                                 exit(1);
                           }
                           else
                           {
                                 colorR=atoi(rgbVec[0].c_str());
                                 if(colorR>255)
                                 {
                                        colorR=255;
                                 }
                                 else if(colorR<0)
                                 {
                                        colorR=0;
                                 }

                                 colorG=atoi(rgbVec[1].c_str());
                                 if(colorG>255)
                                 {
                                        colorG=255;
                                 }
                                 else if(colorG<0)
                                 {
                                        colorG=0;
                                 }

                                 colorB=atoi(rgbVec[2].c_str());
                                 if(colorB>255)
                                 {
                                        colorB=255;
                                 }
                                 else if(colorB<0)
                                 {
                                        colorB=0;
                                 }

                                 i++;

                           }

               }
               else if(arg=="-show")
               {

                     i++;
                     string value(argv[i]);
                     if(value!="on" && value!="off")
                     {
                           cout<<"'"<<value<<"' not defined for show"<<endl;
                           cout<<"type imgwriter -help for options and their values"<<endl;
                           exit(1);
                     }
                     else
                     {
                           if(value=="on")
                           {
                                 show=true;
                           }
                           else if(value=="off")
                           {
                                 show=false;
                           }

                           i++;
                     }

               }
               else if(arg=="-dim")
               {

                     i++;
                     string value(argv[i]);
                     vector<string> whVec;
                     LTKStringUtil::tokenizeString(value, ",", whVec);
                     if(whVec.size()!=1 && whVec.size()!=2)
                     {
                           cout<<"'"<<value<<"' not defined for dim"<<endl;
                           cout<<"type imgwriter -help for options and their values"<<endl;
                           exit(1);
                     }

                     else
                     {
                           if(whVec.size()==1)
                           {

                                 size=atoi(whVec[0].c_str());

                                 dimW = dimH = 0;

                                 if(size<=0)
                                 {
                                        cout<<"Attribute 'size' of dim cannot be less than 1"<<endl;
                                        exit(1);
                                 }
                           }
                           else
                           {

                                 dimW=atoi(whVec[0].c_str());
                                 if(dimW<=0)
                                 {
                                        cout<<"Attribute 'width' of dim cannot be less than 1"<<endl;
                                        exit(1);
                                 }
                                 dimH=atoi(whVec[1].c_str());
                                 if(dimH<=0)
                                 {
                                        cout<<"Attribute 'height' of dim cannot be less than 1"<<endl;
                                        exit(1);
                                 }
                           }

                           i++;

                     }
               }
               else if(arg=="-data")
               {
                     i++;
                     string value(argv[i]);
                     if(value!="unipen" && value!="hpl")
                     {
                           cout<<"'"<<value<<"' not defined for data"<<endl;
                           cout<<"type imgwriter -help for options and their values"<<endl;
                           exit(1);

                     }
                     data=value;
                     i++;

               }
               else if(arg=="-bb")
               {

                     i++;
                     string value(argv[i]);
                     if(value!="true" && value!="false")
                     {
                           cout<<"'"<<value<<"' not defined for bb"<<endl;
                           cout<<"type imgwriter -help for options and their values"<<endl;
                           exit(1);

                     }
                     if(value=="true")
                     {
                           bb=true;
                     }
                     else
                     {
                           bb=false;
                     }
                     i++;
               }
               else if(arg=="-sp")
               {

                     i++;
                     string value(argv[i]);
                     if(value!="true" && value!="false")
                     {
                           cout<<"'"<<value<<"' not defined for sp"<<endl;
                           cout<<"type imgwriter -help for options and their values"<<endl;
                           exit(1);
                     }
                     if(value=="true")
                     {

                           sp=true;
                     }
                     else
                     {
                           sp=false;
                     }
                     i++;
               }
               else if(arg=="-file")
               {
                     i++;
                     string value(argv[i]);
                     filesList=value;
                     i++;
               }
               else if(arg=="-fext")
               {
                     i++;
                     string value(argv[i]);
                     fileExt=value;
                     i++;
               }
               else if ((arg == "-ver")||(arg == "-v"))
               {
                     cout << "\n version of imgwriter tool: " << CURRENT_VERSION << endl;
                     exit(0);
               }
               else
               {
                     src=argv[i];

                     i++;
                     if(i<argc)
                     {
                           dest=argv[i];
                           i++;
                     }
               }

        }


               if(show)
               {
                    startUp(strRootDir,strTempDir);
               }

               if(src.empty())
               {


                    ifstream inputFileList(filesList.c_str());
                    if(!inputFileList)
                    {
                         cout<<"File containing the list of files is not found"<<endl;
                         exit(1);
                    }
                    else
                    {
                         while(inputFileList)
                         {
                              string eachLine;
                              getline(inputFileList,eachLine);
                              if(!eachLine.empty())
                              {
                                   string imgFileName=getFileName(eachLine);

                                   int fileNameIndex=eachLine.rfind(imgFileName);


                                   string destDirectoryPath=eachLine.substr(0,fileNameIndex);
                                   string destImagePath=destDirectoryPath+imgFileName+".bmp";


                                   writeImageFile(eachLine,destImagePath, data, view,
                                           colorR, colorG, colorB, dimW, dimH,
                                           size, bb, sp);

                              }

                         }
                    }

                    inputFileList.close();
               }
               else if(filesList.empty())
               {

                    DIR *srcDIR;
                    struct dirent *srcDIREntry;
                    srcDIR=opendir(src.c_str());
                    if(srcDIR!=NULL)
                    {
                         readdir(srcDIR);
                         readdir(srcDIR);
                         int img=0;
                         string outputFileName=existingTempDir+"/marchtool/mt_tmp/output.html";
                         ofstream output;
                         if(show)
                         {
                              output.open(outputFileName.c_str());
                              output<<"<html>"<<endl;
                              output<<"<body>"<<endl;
                              output<<"<h3 align='center'>March Tool</h3><br><br>"<<endl;
                              output<<"<table align='center' border='1' bordercolor='black'>"<<endl;
                         }

                         while((srcDIREntry=readdir(srcDIR))!=NULL)
                         {

                              string srcFileName(srcDIREntry->d_name);
                              if(srcFileName.rfind(fileExt)!=string::npos)
                              {
                                   string srcFilePath=src+"/"+srcFileName;
                                   string destFile=srcFileName.substr(0,srcFileName.find_last_of('.'))+".bmp";
                                   string destFilePath;
                                   if(show)
                                   {
                                        destFilePath=existingTempDir+"/marchtool/mt_tmp/"+destFile;
                                   }
                                   else
                                   {
                                        if(dest.empty())
                                        {
                                             cout<<"Please mention the destination folder name"<<endl;
                                             exit(1);
                                        }
                                        destFilePath=dest+"/"+destFile;
                                   }
                                   writeImageFile(srcFilePath,destFilePath, data, view,
                                           colorR, colorG, colorB, dimW, dimH,
                                           size,bb,sp);

                                   if(show)
                                   {
                                        if(img==0)
                                        {
                                             output<<"<tr>"<<endl;
                                        }
                                        output<<"<td><a href='file://"<<srcFilePath<<"' title='"<<srcFileName.substr(0,srcFileName.find_last_of('.'))<<"'><img src='"<<destFile<<"' border='0'/></a></td>"<<endl;
                                        img++;

                                        if(img==4)
                                        {
                                             output<<"</tr>"<<endl;
                                             img=0;
                                        }
                                   }

                              }


                         }
                         if(show)
                         {
                              if(img!=4)
                              {
                                   output<<"</tr>"<<endl;
                              }

                              output<<"</table>"<<endl;
                              output<<"</body>"<<endl;
                              output<<"</html>"<<endl;
                              output.close();

                              string exeWithCmdOptions;
#ifdef _WIN32
                              exeWithCmdOptions="rundll32 url.dll,FileProtocolHandler file://"+existingTempDir+"/marchtool/mt_tmp/output.html";
#else
                              exeWithCmdOptions="netscape file:/"+existingTempDir+"/marchtool/mt_tmp/output.html";
#endif

                                     system(exeWithCmdOptions.c_str());

                         }

                    closedir (srcDIR);
               }
               else
               {
                    string title=getFileName(src);

                    string destFile=title+".bmp";
                    string destFilePath;
                    if(show)
                    {
                         destFilePath=existingTempDir+"/marchtool/mt_tmp/"+destFile;
                    }
                    else
                    {
                         if(dest.empty())
                         {
                              cout<<"Please mention the destination file name"<<endl;
                              exit(1);
                         }
                         else
                         {
                              destFilePath=dest;
                         }
                    }

                    writeImageFile(src, destFilePath, data, view, colorR, colorG,
                    colorB,dimW,dimH,size,bb,sp);

                    if(show)
                    {
                         string outputFileName=existingTempDir+"/marchtool/mt_tmp/output.html";
                         ofstream output(outputFileName.c_str());
                         output<<"<html>"<<endl;
                         output<<"<body>"<<endl;
                         output<<"<h3 align='center'>March Tool</h3><br><br>"<<endl;
                         output<<"<table align='center' border='1' bordercolor='black'>"<<endl;
                         output<<"<tr>"<<endl;
                         output<<"<td><a href='file://"<<src<<"' title='"<<title<<"'><img src='"<<destFile<<"' border='0'/></a></td>"<<endl;
                         output<<"</tr>"<<endl;
                         output<<"</table>"<<endl;
                         output<<"</body>"<<endl;
                         output<<"</html>"<<endl;
                         output.close();

                         //system("rundll32 url.dll,FileProtocolHandler file://c:/marchtool/mt_tmp/output.html");
                         string exeWithCmdOptions;
#ifdef _WIN32
                         exeWithCmdOptions="rundll32 url.dll,FileProtocolHandler file://"+existingTempDir+"/marchtool/mt_tmp/output.html";
#else
                         exeWithCmdOptions="netscape file:/"+existingTempDir+"/marchtool/mt_tmp/output.html";
#endif
                         system(exeWithCmdOptions.c_str());

                    }
               }

               }
               else
               {
                    cout<<"Please mention either the source folder/file name or the file containing the list of files"<<endl;
                    exit(1);
               }

     }
     catch(...)
     {
          cout<<endl<<"Unable to execute the command with the specified options!"<<endl;
     }
     return 0;
}
