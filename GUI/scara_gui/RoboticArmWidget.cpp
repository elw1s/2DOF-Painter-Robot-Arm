// RoboticArmWidget.cpp

#include "RoboticArmWidget.h"
#include <Qt3DExtras/QOrbitCameraController>

RoboticArmWidget::RoboticArmWidget(QWidget *parent)
    : QWidget(parent), m_view(new Qt3DExtras::Qt3DWindow()) {

    m_rootEntity = new Qt3DCore::QEntity();

    setupArm();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(QWidget::createWindowContainer(m_view, this));
    setLayout(layout);
}

void RoboticArmWidget::setupArm() {
    // Create entities for each part of the arm
    Qt3DCore::QEntity *shoulderEntity = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DCore::QEntity *elbowEntity = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DCore::QEntity *liftingEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_shoulderEntity = shoulderEntity;
    m_elbowEntity = elbowEntity;
    m_liftingEntity = liftingEntity;
    // Add transform components for positioning the arm parts
    Qt3DCore::QTransform *shoulderTransform = new Qt3DCore::QTransform();
    shoulderTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f)); // Position of the shoulder
    shoulderEntity->addComponent(shoulderTransform);

    Qt3DCore::QTransform *elbowTransform = new Qt3DCore::QTransform();
    elbowTransform->setTranslation(QVector3D(0.0f, 0.0f, 1.0f)); // Position of the elbow relative to shoulder
    elbowEntity->addComponent(elbowTransform);

    Qt3DCore::QTransform *liftingTransform = new Qt3DCore::QTransform();
    liftingTransform->setTranslation(QVector3D(0.0f, 0.0f, 2.0f)); // Position of the lifting segment relative to elbow
    liftingEntity->addComponent(liftingTransform);

    // Create cylinder meshes for arm segments
    Qt3DExtras::QCylinderMesh *shoulderMesh = new Qt3DExtras::QCylinderMesh();
    shoulderMesh->setRadius(0.2f);
    shoulderMesh->setLength(1.0f); // Length of the shoulder segment
    shoulderEntity->addComponent(shoulderMesh);

    Qt3DExtras::QCylinderMesh *elbowMesh = new Qt3DExtras::QCylinderMesh();
    elbowMesh->setRadius(0.15f);
    elbowMesh->setLength(1.0f); // Length of the elbow segment
    elbowEntity->addComponent(elbowMesh);

    Qt3DExtras::QCylinderMesh *liftingMesh = new Qt3DExtras::QCylinderMesh();
    liftingMesh->setRadius(0.1f);
    liftingMesh->setLength(1.0f); // Length of the lifting segment
    liftingEntity->addComponent(liftingMesh);

    // Set materials for the arm segments (you might want to create different materials for each segment)
    Qt3DExtras::QPhongMaterial *armMaterial = new Qt3DExtras::QPhongMaterial();
    armMaterial->setDiffuse(QColor(Qt::green)); // Set the arm color
    shoulderEntity->addComponent(armMaterial);
    elbowEntity->addComponent(armMaterial);
    liftingEntity->addComponent(armMaterial);

    // Set parenting relationships for segments
    elbowEntity->setParent(shoulderEntity);
    liftingEntity->setParent(elbowEntity);

    // Set up camera controller
    Qt3DExtras::QOrbitCameraController *cameraController = new Qt3DExtras::QOrbitCameraController(m_rootEntity);
    cameraController->setLinearSpeed(2.0f);
    cameraController->setLookSpeed(100.0f);
    cameraController->setCamera(m_view->camera());
    m_view->activeFrameGraph()->addCamera(m_view->camera());
}
void RoboticArmWidget::mousePressEvent(QMouseEvent *event) {
    Qt3DRender::QPickEvent *pickEvent = dynamic_cast<Qt3DRender::QPickEvent*>(event);
    if (!pickEvent)
        return;

    Qt3DCore::QEntity *clickedEntity = pickEvent->entity();
    if (clickedEntity) {
        // Identify the clicked part based on the clicked entity and emit signal with part name
        QString partName;

        if (clickedEntity == m_shoulderEntity)
            partName = "Shoulder";
        else if (clickedEntity == m_elbowEntity)
            partName = "Elbow";
        else if (clickedEntity == m_liftingEntity)
            partName = "Lifting";

        if (!partName.isEmpty()) {
            emit partClicked(partName);
        }
    }
}
