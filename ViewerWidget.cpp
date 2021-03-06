#include "ViewerWidget.h"
#include "QDebug"

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) :
    QWidget() {
    setThreadingModel(threadingModel);

    _view = NULL;
    _camera = NULL;
    _widget = NULL;
//    QWidget* widget1 = addViewWidget( createCamera(0,0,100,100), osgDB::readNodeFile("../LEGO_Creator/OSG/cow.osg") );

//    widget1->show();

//    QVBoxLayout* mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(widget1);
//    setLayout(mainLayout);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

//QWidget* ViewerWidget::addViewWidget( osg::Camera* camera, osg::Node* scene ) {
//    osgViewer::View* view = new osgViewer::View;
//    view->setCamera( camera );
//    addView( view );

//    view->setSceneData( scene );
//    view->addEventHandler( new osgViewer::StatsHandler );
//    view->setCameraManipulator( new osgGA::TrackballManipulator );

//    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
//    return gw ? gw->getGLWidget() : NULL;
//}

ViewerWidget::~ViewerWidget() {
}

osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, const std::string& name, bool windowDecoration) {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );

    camera->setClearColor( osg::Vec4(0.2, 0.2, 0.2, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
                30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    return camera.release();
}

void ViewerWidget::initView(void) {
    _view = new osgViewer::View;
    addView(_view);
}

void ViewerWidget::changeCamera(osg::Camera* camera) {
    _camera = camera;
    _view->setCamera(_camera);
}

void ViewerWidget::changeScene(osg::Node* scene) {
    _view->setSceneData(scene);
    _view->addEventHandler(new osgViewer::StatsHandler);
    _view->setCameraManipulator(new osgGA::TrackballManipulator);
}

void ViewerWidget::initWidget(void) {
    if (osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(_camera->getGraphicsContext())) {
        _widget = gw->getGLWidget();
        _widget->show();
    } else
        qDebug() << "Cannot set ViewerWidget widget in ViewerWidget::setWidget(QWidget* widget)";

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_widget);
    setLayout(mainLayout);
}
