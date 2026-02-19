#include "src/candle/drawers/meshdrawer.h"
#include <limits>

using namespace candle::geometry;

MeshDrawer::MeshDrawer()
    : ShaderDrawable(), m_hasMesh(false)
{
}

MeshDrawer::~MeshDrawer()
{
    // cleanup handled by base
}

void MeshDrawer::setMesh(const Mesh &mesh)
{
    m_mesh = mesh;
    m_hasMesh = true;
    // mark geometry update
    update();
}

void MeshDrawer::clearMesh()
{
    m_mesh.vertices.clear();
    m_mesh.triangles.clear();
    m_hasMesh = false;
    update();
}

QVector3D MeshDrawer::getViewRanges()
{
    if (!m_hasMesh) return QVector3D();
    auto b = m_mesh.bounds();
    if (!b) return QVector3D();
    auto mn = b->first; auto mx = b->second;
    return QVector3D(mx.x - mn.x, mx.y - mn.y, mx.z - mn.z);
}

QVector3D MeshDrawer::getViewLowerBounds()
{
    if (!m_hasMesh) return QVector3D();
    auto b = m_mesh.bounds();
    return QVector3D(b->first.x, b->first.y, b->first.z);
}

QVector3D MeshDrawer::getViewUpperBounds()
{
    if (!m_hasMesh) return QVector3D();
    auto b = m_mesh.bounds();
    return QVector3D(b->second.x, b->second.y, b->second.z);
}

QVector3D MeshDrawer::getModelRanges()
{
    return getViewRanges();
}

QVector3D MeshDrawer::getModelLowerBounds()
{
    return getViewLowerBounds();
}

QVector3D MeshDrawer::getModelUpperBounds()
{
    return getViewUpperBounds();
}

int MeshDrawer::getVertexCount()
{
    if (!m_hasMesh) return 0;
    return static_cast<int>(m_mesh.triangles.size() * 3);
}

bool MeshDrawer::updateData()
{
    // Fill m_triangles with mesh vertex positions and a default color
    if (!m_hasMesh) return false;

    m_triangles.clear();
    m_triangles.reserve(m_mesh.triangles.size() * 3);

    for (const auto &t : m_mesh.triangles) {
        for (int k = 0; k < 3; ++k) {
            const auto &v = m_mesh.vertices[t[k]];
            VertexData vd;
            vd.position = QVector3D(v.x, v.y, v.z);
            vd.color = QVector3D(0.8f, 0.8f, 0.8f);
            vd.data = QVector3D(0,0,0);
            vd.type = VertexDataTypeTriangle;
            m_triangles.append(vd);
        }
    }

    // VBO update is handled by base class using m_triangles
    return true;
}
