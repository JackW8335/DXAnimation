#include "Exporter.h";

Exporter::Exporter()
{
	fout.open("../DirectXEngineAT/export/model.obj");

	fout << "mtllib model.mtl";
	fout << endl;

	//fout.close();
}

void Exporter::exportModel(std::vector <ModelClass*> m_Model)
{
	int last_index = 0;
	float posX, posY, posZ;

	for (int s = 0; s < m_Model.size(); s++)
	{
		fout << "g " << m_Model[s]->getName();
		fout << endl;

		for (int i = 0; i < m_Model[s]->GetIndexCount(); i++)
		{
			
			m_Model[s]->GetPosition(posX, posY, posZ);
			fout << "v " << m_Model[s]->GetModel()[i].x + posX*2 << ' ' << m_Model[s]->GetModel()[i].y + posY*2 
				<< ' ' << m_Model[s]->GetModel()[i].z + posZ*2 << ' ' << endl;
		}

		for (int i = 0; i < m_Model[s]->GetIndexCount(); i++)
		{ 
			fout << "vn " << m_Model[s]->GetModel()[i].nx << ' ' << m_Model[s]->GetModel()[i].ny << ' ' << m_Model[s]->GetModel()[i].nz << endl;
		}

		for (int i = 0; i < m_Model[s]->GetIndexCount(); i++)
		{
			fout << "vt " << m_Model[s]->GetModel()[i].tu << ' ' << m_Model[s]->GetModel()[i].tv << ' ' << endl;
		}

		fout << "usemtl model";
		fout << endl;

		for (int t = 0; t < m_Model[s]->GetIndexCount(); t += 3)
		{
			int idx2 = t + 1 + last_index;
			int idx1 = t + 1 + 1 + last_index;
			int idx0 = t + 2 + 1 + last_index;


			fout << "f " + FaceString(idx2) + " " + FaceString(idx1) + " " + FaceString(idx0);
			fout << endl;
			
		}
		last_index += m_Model[s]->GetIndexCount();
	}

	// Close the output file.
	fout.close();

	fout.open("../DirectXEngineAT/export/model.mtl");

	for (int i = 0; i < m_Model.size(); i++)
	{
		fout << "newmtl model";
		fout << endl;
		fout << "map_Kd " << m_Model[i]->GetTextureObj()->GetTexturePath();
		fout << endl;
	}
	fout.close();
}

string Exporter::FaceString(int index)
{
	string idxString = std::to_string(index);

	return idxString + "/" + idxString + "/" + idxString;
}



