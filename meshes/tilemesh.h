#ifndef TILEMESH_H
#define TILEMESH_H

#include <QPlaneMesh>

class TileMesh : public Qt3DExtras::QPlaneMesh
{
    Q_OBJECT
public:
    explicit TileMesh(Qt3DCore::QNode *parent = 0);
    ~TileMesh();
};

#endif // TILEMESH_H
