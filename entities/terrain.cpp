#include "terrain.h"

Entity::Terrain::Terrain(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_cameraController()
{
}

Entity::Terrain::Terrain(CameraController *cameraController, Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_cameraController(cameraController)
{
    init();
}

Entity::Terrain::~Terrain()
{
    // FIXME: Correct free object
    for (auto tile : m_tiles) {
        delete tile;
    }
}

CameraController* Entity::Terrain::cameraController() const
{
    return m_cameraController;
}

void Entity::Terrain::setCameraController(CameraController *cameraController)
{
    m_cameraController = cameraController;
}

void Entity::Terrain::init()
{
    Tile::TileCoords tileCoords = m_cameraController->getTileCoords();

    Tile *tile = new Tile(tileCoords, this);
    m_tiles.append(tile);
    // TODO: add to pyramid
}
