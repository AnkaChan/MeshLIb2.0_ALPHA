#pragma once
#include<iostream>
#include<functional>
#include<string>
#include<io.h>
#include<vector>
#include<Windows.h>
namespace MeshLib{

	struct FileParts
	{
		std::string path;
		std::string name;
		std::string ext;
	};
	std::string standardSlash(std::string oPath) {
		std::replace(oPath.begin(), oPath.end(), '\\', '/');
		return oPath;
	}
	std::string make2StandardPath(std::string oPath) {
		oPath = standardSlash(oPath); // replace all '\' to '/', fuck Microsophtte
		if (oPath.back() != '/') {
			oPath.push_back('/');
		}
		return oPath;
	}



	static FileParts fileparts(std::string filename)
	{
		int idx0 = filename.rfind("/");
		int idx1 = filename.rfind(".");

		FileParts fp;
		fp.path = filename.substr(0, idx0 + 1);
		fp.name = filename.substr(idx0 + 1, idx1 - idx0 - 1);
		fp.ext = filename.substr(idx1);

		return fp;
	}

	//读取某给定路径下所有文件夹与文件名称，并带完整路径。
	static void getAllFiles(std::string path, std::vector<std::string>& files) {
		//文件句柄
		long long hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;  //很少用的文件信息读取结构
		std::string p;  //std::string类很有意思的一个赋值函数:assign()，有很多重载版本
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR)) {  //比较文件类型是否是文件夹
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
						getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
					}
				}
				else {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
			_findclose(hFile);
		}
	}
	static void getJustCurrentDir(std::string path, std::vector<std::string>& files) {
		//文件句柄
		long long hFile = 0;
		//文件信息 
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						files.push_back(fileinfo.name);
						//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					}
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	static void getJustCurrentFile(std::string path, std::vector<std::string>& files) {
		//文件句柄
		long long hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR)) {
					;
				}
				else {
					//files.push_back(fileinfo.name);
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	static void getFilesAll(std::string path, std::vector<std::string>& files) {
		//文件句柄
		long long hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
						getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
					}
				}
				else {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
	
}