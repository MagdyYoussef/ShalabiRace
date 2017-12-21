#ifndef NOOBS_H
#define NOOBS_H
#include <string>
#include <vector>

struct NoobModel
{
	string m_modelFileName;
	string m_texFileName;
};

NoobModel NoobModels[1] = {
	{"Models/test/tris1.MD2", "Models/test/MEGAMAN.PCX"}
};

#endif