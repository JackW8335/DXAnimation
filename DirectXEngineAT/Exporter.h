#pragma once
#include <fstream>;
#include "modelclass.h"
#include <string>
class Exporter
{
public:
	Exporter();
	~Exporter() = default;
	void exportModel(std::vector <ModelClass*>);
private:
	string FaceString(int index);
	ModelClass* m_Model;
	std::ofstream fout;
};