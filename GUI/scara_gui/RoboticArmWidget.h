// RoboticArmWidget.h
#ifndef ROBOTICARMWIDGET_H
#define ROBOTICARMWIDGET_H

#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <QMouseEvent>

class RoboticArmWidget : public QWidget {
    Q_OBJECT
public:
    explicit RoboticArmWidget(QWidget *parent = nullptr);

signals:
    void partClicked(const QString &partName);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_shoulderEntity;
    Qt3DCore::QEntity *m_elbowEntity;
    Qt3DCore::QEntity *m_liftingEntity;

    void setupArm();
};

#endif // ROBOTICARMWIDGET_H
