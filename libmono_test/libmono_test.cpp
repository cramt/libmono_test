#include <iostream>
#include <tchar.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <windows.h>
#include <io.h>     // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <iostream> 
#include <list> 
#include <iterator> 
#include <comdef.h>  
#include <string>
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>


using namespace std;

string ExePath() {
	char buffer[260];
	GetModuleFileNameA(NULL, buffer, 260);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

bool DirectoryExists(string _absolutePath) {
	const char* absolutePath = _absolutePath.c_str();

	if (_access(absolutePath, 0) == 0) {

		struct stat status;
		stat(absolutePath, &status);

		return (status.st_mode & S_IFDIR) != 0;
	}
	return false;
}

string findPackagesFolder(string path) {
	size_t index = path.find_last_of("\\");
	if (index == -1) {
		return "";
	}
	string folderName = path.substr(index + 1);
	string parentPath = path.substr(0, index);
	if (DirectoryExists(parentPath + "\\packages")) {
		return parentPath + "\\packages";
	}
	return findPackagesFolder(parentPath);
}

list<string> readfolder(string folder){
	list<string> res;
	string a = folder + "\\*";
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wstring(a.begin(), a.end()).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			string str = string(_bstr_t(data.cFileName));
			if (str != "." && str != "..") {
				res.push_back(str);
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	return res;
}


MonoDomain* domain;
MonoAssembly* assembly;

int main() {
	/*
	string a = findPackagesFolder(ExePath());
	if (a == "") {
		cout << "aaaa";
	}
	else {
		get_all_files_names_within_folder(a);
	}




	return 0;
	*/

	mono_set_dirs("D:\\programs\\Mono\\lib", "C:\\Mono-2.6.7\\etc");
	domain = mono_jit_init("test");
	cout << "a"; 
	string __dirname = ExePath();
	list<string> dep = readfolder(__dirname + "\\dll_dep");

	for (list<string>::iterator it = dep.begin(); it != dep.end(); ++it) {
		mono_domain_assembly_open(domain, (__dirname + "\\dll_dep\\" + *it).c_str());
	}
	assembly = mono_domain_assembly_open(domain, (__dirname + "\\dotnetTest.exe").c_str());
	cout << "b";
	MonoImage* image = mono_assembly_get_image(assembly);
	cout << "c";
	MonoClass* testClass = mono_class_from_name(image, "dotnetTest", "TestClass");
	cout << "d";
	MonoObject* object = mono_object_new(domain, testClass);
	cout << "e";
	mono_runtime_object_init(object);
	cout << "f";

	void* iter = NULL;
	MonoMethod* gimme, * m, * ctor;
	gimme = NULL;
	while ((m = mono_class_get_methods(testClass, &iter))) {
		if (strcmp(mono_method_get_name(m), "Gimme") == 0) {
			gimme = m;
		}
		else if (strcmp(mono_method_get_name(m), ".ctor") == 0) {
			ctor = m;
		}
	}

	cout << "g";
	mono_runtime_object_init(object);
	cout << "h";
	void* args[1];
	args[0] = mono_string_new(domain, "yare yare daze");
	cout << "i";
	MonoObject* returnValue = mono_runtime_invoke(gimme, object, args, NULL);
	cout << "j";
	cout << mono_string_to_utf8(mono_object_to_string(returnValue, NULL)) << endl;
	cout << "k";
	return 0;
}
