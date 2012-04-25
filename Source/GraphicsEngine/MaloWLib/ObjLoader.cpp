#include "ObjLoader.h"

D3DXVECTOR3 ObjLoader::GetVertPosData(string line)
{
	string sx = "";
	string sy = "";
	string sz = "";

	size_t spacepos = line.find(" ");

	sx = line.substr(spacepos + 1);
	line = sx;

	spacepos = line.find(" ");

	sy = line.substr(spacepos + 1);
	line = sy;

	spacepos = line.find(" ");

	sz = line.substr(spacepos + 1);

	float x = (float)atof(sx.c_str());
	float y = (float)atof(sy.c_str());
	float z = (float)atof(sz.c_str());
				
	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR2 ObjLoader::GetTextCoordsData(string line)
{
	string su = "";
	string sv = "";

	size_t spacepos = line.find(" ");

	su = line.substr(spacepos + 1);
	line = su;

	spacepos = line.find(" ");

	sv = line.substr(spacepos + 1);

	float u = (float)atof(su.c_str());
	float v = (float)atof(sv.c_str());

	v = 1 - v;	// For D3DX

	return D3DXVECTOR2(u, v);
}

D3DXVECTOR3 ObjLoader::GetVertsNormsData(string line)
{
	string sx = "";
	string sy = "";
	string sz = "";

	size_t spacepos = line.find(" ");

	sx = line.substr(spacepos + 1);
	line = sx;

	spacepos = line.find(" ");

	sy = line.substr(spacepos + 1);
	line = sy;

	spacepos = line.find(" ");

	sz = line.substr(spacepos + 1);

	float x = (float)atof(sx.c_str());
	float y = (float)atof(sy.c_str());
	float z = (float)atof(sz.c_str());

	return D3DXVECTOR3(x, y, z);
}

FaceData ObjLoader::GetFaceData(string line)
{
	size_t spacepos = line.find(" ");
	line = line.substr(spacepos + 1);

	string a[3];

	spacepos = line.find(" ");
	a[0] = line.substr(0, spacepos);
	line = line.substr(spacepos+1);

	spacepos = line.find(" ");
	a[1] = line.substr(0, spacepos);
	line = line.substr(spacepos+1);

	a[2] = line;

	FaceData face;
	for(int i = 0; i < 3; i++)
	{
		string b[3];

		size_t slashpos = a[i].find("/");
		b[0] = a[i].substr(0, slashpos);
		a[i] = a[i].substr(slashpos + 1);

		slashpos = a[i].find("/");
		b[1] = a[i].substr(0, slashpos);
		a[i] = a[i].substr(slashpos + 1);
					
		slashpos = a[i].find("/");
		b[2] = a[i].substr(0, slashpos);
		a[i] = a[i].substr(slashpos + 1);

		for(int u = 0; u < 3; u++)
		{
			face.data[i][u] = atoi(b[u].c_str());
		}
	}
	return face;
}

void ObjLoader::trianglulate(string& filename)
{
	ifstream file;
	file.open(filename);
	if(!file)
	{
		MaloW::Debug("Couldnt open file in ObjLoader: " + filename);
	}

	bool needsTrianglulation = false;
	while(!file.eof())
	{
		string line = "";
		getline(file, line);

		if(line.size() > 0 && line.at(0) == 'f')
		{
			line = line.substr(1);
			int NrOfSlashes = 0;
			int pos = 0;
			while(pos < (int)line.length())
			{
				if(line[pos] == '/')
					NrOfSlashes++;
				pos++;
			}
			if(NrOfSlashes > 6)
			{
				needsTrianglulation = true;
			}
		}
	}
	file.close();
	
	if(needsTrianglulation)
	{
		file.open(filename);
		filename = "Triangulated" + filename;
		ofstream trifile;
		trifile.open(filename);
		while(!file.eof())
		{
			string line = "";
			getline(file, line);

			if(line.size() > 0 && line.at(0) == 'f')
			{
				int NrOfSlashes = 0;
				int pos = 0;
				while(pos < (int)line.length())
				{
					if(line[pos] == '/')
						NrOfSlashes++;
					pos++;
				}
				if(NrOfSlashes > 6)
				{
					string newline = line.substr(1);


					size_t spacepos = newline.find(" ");
					newline = newline.substr(spacepos + 1);

					spacepos = newline.find(" ");
					newline = newline.substr(spacepos+1);

					trifile << "f " + newline + "\n";

					spacepos = newline.find(" ");
					newline = newline.substr(spacepos+1);

					spacepos = newline.find(" ");
					newline = newline.substr(spacepos+1);

					line = line.substr(0, line.length() - newline.length());
				}
			}

			trifile << line + "\n";
		}
		file.close();
		trifile.close();
	}
}

ObjData* ObjLoader::LoadObjFile(string filepath)
{
	string mtlfile = "";
	ObjData* returndata = new ObjData();
	//this->trianglulate(filename);								////////// Not quite working

	string folders = "";
	string filename = filepath;
	size_t slashpos = filename.find("/");
	while(slashpos != string::npos)
	{
		slashpos = filename.find("/");
		folders += filename.substr(0, slashpos + 1);
		filename = filename.substr(slashpos + 1);
	}

	
	// Binary file
	ifstream binfile;
	binfile.open(folders + "Cache/" + filename.substr(0, filename.size() - 4) + ".MalEng", ios::binary);
	if(binfile)
	{
		this->ReadFromBinaryFile(returndata, binfile);
		binfile.close();
		return returndata;			// Return and skip the Obj-file.
	}
	


	// Obj file
	ifstream file;
	file.open(filepath);
	if(!file)
	{
		MaloW::Debug("Couldnt open file in ObjLoader: " + filepath);
	}

	string currentMaterial = "";

	while(!file.eof())
	{
		string line = "";
		getline(file, line);
		if(line.size() > 0 && line.substr(0, 2) == "v ")
		{
			D3DXVECTOR3 vertpos = this->GetVertPosData(line);
			vertpos.z *= -1;	// For D3DX
			returndata->vertspos->add(vertpos);
		}
		else if(line.size() > 0 && line.substr(0, 2) == "vt")
		{
			returndata->textcoords->add(this->GetTextCoordsData(line));
		}
		else if(line.size() > 0 && line.substr(0, 2) == "vn")
		{
			D3DXVECTOR3 norm = this->GetVertsNormsData(line);
			norm.z *= -1;	// For D3DX
			returndata->vertsnorms->add(norm);
		}
		else if(line.size() > 0 && line.at(0) == 'f')
		{
			FaceData data = this->GetFaceData(line);
			data.material = currentMaterial;
			returndata->faces->add(data);
		}
		else if(line.size() > 0 && line.substr(0, 6) == "mtllib")
			mtlfile = line.substr(7);

		else if(line.size() > 0 && line.substr(0, 6) == "usemtl")
			currentMaterial = line.substr(7);
	}
	file.close();


	
	// mtl file
	if(mtlfile == "")
		return returndata;
	
	mtlfile = folders + mtlfile;
	

	file.open(mtlfile);
	if(!file)
	{
		MaloW::Debug("Couldnt open file in ObjLoader: " + mtlfile);
	}

	bool firstMat = true;
	MaterialData md;
	while(!file.eof())
	{
		string line = "";
		getline(file, line);

		if(line.size() > 0 && line.substr(0, 6) == "newmtl")
		{
			if(!firstMat)
			{
				returndata->mats->add(md);
				md = MaterialData();
			}
			md.name = line.substr(7);
			firstMat = false;
		}
		else if(line.size() > 0 && line.substr(0, 5) == "illum")
		{
			md.illum = atoi(line.substr(6).c_str());
		}
		else if(line.size() > 0 && line.substr(0, 2) == "Kd")
		{
			D3DXVECTOR3 Kd = this->GetVertPosData(line);
			md.kd = Kd;
		}
		else if(line.size() > 0 && line.substr(0, 2) == "Ka")
		{
			D3DXVECTOR3 Ka = this->GetVertPosData(line);
			md.ka = Ka;
		}
		else if(line.size() > 0 && line.substr(0, 2) == "Tf")
		{
			D3DXVECTOR3 Tf = this->GetVertPosData(line);
			md.tf = Tf;
		}
		else if(line.size() > 0 && line.substr(0, 6) == "map_Kd")
		{
			md.texture = folders;
			md.texture += line.substr(7);
		}
		else if(line.size() > 0 && line.substr(0, 2) == "Ks")
		{
			D3DXVECTOR3 Ks = this->GetVertPosData(line);
			md.ks = Ks;
		}
		else if(line.size() > 0 && line.substr(0, 2) == "Ni")
		{
			float Ni = (float)atof(line.substr(3).c_str());
			md.ni = Ni;
		}

	}
	if(!firstMat)
		returndata->mats->add(md);
	
	file.close();

	this->CreateBinaryFile(folders + "Cache/" + filename, returndata);

	return returndata;
}


void ObjLoader::ReadFromBinaryFile(ObjData* returndata, ifstream& binfile)
{
	int nrOfVerts = 0;
	binfile.read((char*)(&nrOfVerts), sizeof(int));

	int nrOfTextCoords = 0;
	binfile.read((char*)(&nrOfTextCoords), sizeof(int));

	int nrOfNorms = 0;
	binfile.read((char*)(&nrOfNorms), sizeof(int));

	int nrOfFaces = 0;
	binfile.read((char*)(&nrOfFaces), sizeof(int));

	int nrOfMats = 0;
	binfile.read((char*)(&nrOfMats), sizeof(int));

	for(int i = 0; i < nrOfVerts; i++)
	{
		float x = 0;
		float y = 0;
		float z = 0;

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));

		returndata->vertspos->add(D3DXVECTOR3(x, y, z));
	}

	for(int i = 0; i < nrOfTextCoords; i++)
	{
		float x = 0;
		float y = 0;

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));

		returndata->textcoords->add(D3DXVECTOR2(x, y));
	}

	for(int i = 0; i < nrOfNorms; i++)
	{
		float x = 0;
		float y = 0;
		float z = 0;

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));

		returndata->vertsnorms->add(D3DXVECTOR3(x, y, z));
	}

	for(int i = 0; i < nrOfFaces; i++)
	{
		char materialName[50] = {0};
		binfile.read((char*)(&materialName), sizeof(materialName));

		FaceData fd;
			
		for(int u = 0; u < 3; u++)
		{
			for(int y = 0; y < 3; y++)
			{
				binfile.read((char*)(&fd.data[u][y]), sizeof(int));
			}
		}
		fd.material = materialName;
		returndata->faces->add(fd);
	}

	for(int i = 0; i < nrOfMats; i++)
	{
		char materialName[50] = {0};
		binfile.read((char*)(&materialName), sizeof(materialName));
		char texture[50] = {0};
		binfile.read((char*)(&texture), sizeof(texture));

		MaterialData md;
		md.name = materialName;
		md.texture = texture;
			
		binfile.read((char*)(&md.illum), sizeof(int));

		float x = 0; 
		float y = 0; 
		float z = 0;

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));
		md.kd = D3DXVECTOR3(x, y, z);

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));
		md.ka = D3DXVECTOR3(x, y, z);

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));
		md.tf = D3DXVECTOR3(x, y, z);

		binfile.read((char*)(&x), sizeof(float));
		binfile.read((char*)(&y), sizeof(float));
		binfile.read((char*)(&z), sizeof(float));
		md.ks = D3DXVECTOR3(x, y, z);

		binfile.read((char*)(&md.ni), sizeof(float));

		returndata->mats->add(md);
	}
}

void ObjLoader::CreateBinaryFile(string filename, ObjData* returndata)
{
	CreateDirectory("Media\\Cache\\", NULL);
	string binfilename = filename.substr(0, filename.length()-4);
	ofstream binfile;
	binfile.open(binfilename + ".MalEng", ios::binary);
	if(!binfile)
	{
		MaloW::Debug("Couldnt create " + binfilename + ".MalEng" + " when trying to create binary file from obj-file");
		return;
	}

	int nrOfVerts = returndata->vertspos->size();
	binfile.write((char*)(&nrOfVerts), sizeof(int));

	int nrOfTextCoords = returndata->textcoords->size();
	binfile.write((char*)(&nrOfTextCoords), sizeof(int));

	int nrOfNorms = returndata->vertsnorms->size();
	binfile.write((char*)(&nrOfNorms), sizeof(int));

	int nrOfFaces = returndata->faces->size();
	binfile.write((char*)(&nrOfFaces), sizeof(int));

	int nrOfMats = returndata->mats->size();
	binfile.write((char*)(&nrOfMats), sizeof(int));

	for(int i = 0; i < nrOfVerts; i++)
	{
		float x = returndata->vertspos->get(i).x;
		float y = returndata->vertspos->get(i).y;
		float z = returndata->vertspos->get(i).z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

	}

	for(int i = 0; i < nrOfTextCoords; i++)
	{
		float x = returndata->textcoords->get(i).x;
		float y = returndata->textcoords->get(i).y;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));

	}

	for(int i = 0; i < nrOfNorms; i++)
	{
		float x = returndata->vertsnorms->get(i).x;
		float y = returndata->vertsnorms->get(i).y;
		float z = returndata->vertsnorms->get(i).z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

	}

	for(int i = 0; i < nrOfFaces; i++)
	{
		FaceData fd = returndata->faces->get(i);
		char materialName[50] = {0};
		for(int u = 0; u < (int)fd.material.size(); u++)
			materialName[u] = fd.material[u];

		binfile.write((char*)(&materialName), sizeof(materialName));

			
		for(int u = 0; u < 3; u++)
		{
			for(int y = 0; y < 3; y++)
			{
				binfile.write((char*)(&fd.data[u][y]), sizeof(int));
			}
		}
	}

	for(int i = 0; i < nrOfMats; i++)
	{
		MaterialData md = returndata->mats->get(i);
		char materialName[50] = {0};
		for(int u = 0; u < (int)md.name.size(); u++)
			materialName[u] = md.name[u];

		binfile.write((char*)(&materialName), sizeof(materialName));


		char texture[50] = {0};
		for(int u = 0; u < (int)md.texture.size(); u++)
			texture[u] = md.texture[u];

		binfile.write((char*)(&texture), sizeof(texture));

		binfile.write((char*)(&md.illum), sizeof(int));

		float x = md.kd.x; 
		float y = md.kd.y; 
		float z = md.kd.z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

		x = md.ka.x; 
		y = md.ka.y; 
		z = md.ka.z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

		x = md.tf.x; 
		y = md.tf.y; 
		z = md.tf.z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

		x = md.ks.x; 
		y = md.ks.y; 
		z = md.ks.z;

		binfile.write((char*)(&x), sizeof(float));
		binfile.write((char*)(&y), sizeof(float));
		binfile.write((char*)(&z), sizeof(float));

		binfile.write((char*)(&md.ni), sizeof(float));
	}
	binfile.close();
}