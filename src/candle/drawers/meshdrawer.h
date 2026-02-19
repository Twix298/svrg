#pragma once

#include "../geometry/mesh_importer.h"
#include "shaderdrawable.h"

/// @brief Drawer for rendering triangle meshes using ShaderDrawable pipeline.
class MeshDrawer : public ShaderDrawable
{
public:
    MeshDrawer();
    ~MeshDrawer();

    /// @brief Set mesh data to display.
    void setMesh(const candle::geometry::Mesh &mesh);

    /// @brief Clear current mesh.
    void clearMesh();

    // Overrides
    virtual QVector3D getViewRanges() override;
    virtual QVector3D getViewLowerBounds() override;
    virtual QVector3D getViewUpperBounds() override;

    virtual QVector3D getModelRanges() override;
    virtual QVector3D getModelLowerBounds() override;
    virtual QVector3D getModelUpperBounds() override;

    virtual int getVertexCount() override;

protected:
    virtual bool updateData() override;

private:
    candle::geometry::Mesh m_mesh;
    bool m_hasMesh;
};
