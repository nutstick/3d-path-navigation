#include "tilemesh.h"

#include <QGeometry>
#include <QAttribute>

//class TessellatedGeometry : public Qt3DRender::QGeometry {
//    Q_OBJECT
//public:
//    TessellatedGeometry(Qt3DCore::QNode *parent = Q_NULLPTR)
//        : Qt3DRender::QGeometry(parent)
//        , m_positionAttribute(new Qt3DRender::QAttribute(this))
//        , m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
//    {
//        const float positionData[] = {
//            -0.8f, -0.8f, 0.0f,
//            0.8f, -0.8f, 0.0f,
//            0.8f, 0.8f, 0.0f,
//            -0.8f, 0.8f, 0.0f
//        };

//        const int nVerts = 4;
//        const int size = nVerts * 3 * sizeof(float);
//        QByteArray positionBytes;
//        positionBytes.resize(size);
//        memcpy(positionBytes.data(), positionData, size);

//        m_vertexBuffer->setData(positionBytes);

//        m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
//        m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
//        m_positionAttribute->setDataSize(3);
//        m_positionAttribute->setCount(nVerts);
//        m_positionAttribute->setByteStride(3 * sizeof(float));
//        m_positionAttribute->setBuffer(m_vertexBuffer);

//        setVerticesPerPatch(4);
//        addAttribute(m_positionAttribute);
//    }

//private:
//    Qt3DRender::QAttribute *m_positionAttribute;
//    Qt3DRender::QBuffer *m_vertexBuffer;
//};

TileMesh::TileMesh(Qt3DCore::QNode *parent)
    : Qt3DExtras::QPlaneMesh(parent)
{
    // setPrimitiveType(Qt3DRender::QGeometryRenderer::Patches);
    //    setGeometry(new TessellatedGeometry(this));
}

TileMesh::~TileMesh()
{
}

// #include "tilemesh.moc"
