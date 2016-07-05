#include <windows.h>
#include <osg/ArgumentParser>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <time.h>

class JetSmokeStateSetUpdate : public osg::StateSet::Callback
{
public:
	JetSmokeStateSetUpdate()
		: time(0.0f)
	{
	}
	virtual void operator () (osg::StateSet * ss, osg::NodeVisitor *nv)
	{
		GetSystemTime(&ts);
		time = (float)(ts.wSecond % 30) + (float)ts.wMilliseconds / 1000.0f;
		ss->getUniform(std::string("time"))->set(time);
	}
protected:
	SYSTEMTIME ts;
	float time;
};

int
main(int argc, char ** argv)
{
	osg::ArgumentParser arguments(&argc, argv);

	// create a stack of 100 textured quads
	osg::Vec3Array * coords = new osg::Vec3Array(400);
	for (int i = 0; i < 100; i++)
	{
		coords[0][i * 4].set(-1, -1, float(i) / 100.0);
		coords[0][i * 4 + 1].set(1, -1, float(i) / 100.0);
		coords[0][i * 4 + 2].set(1, 1, float(i) / 100.0);
		coords[0][i * 4 + 3].set(-1, 1, float(i) / 100.0);
	}

	osg::Geometry * geometry = new osg::Geometry;
	geometry->setVertexArray(coords);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS, 0, coords->size()));
	geometry->getOrCreateStateSet()->addUniform(new osg::Uniform("time", 0.0f));
	geometry->getOrCreateStateSet()->setUpdateCallback(new JetSmokeStateSetUpdate);

	osg::Geode * g = new osg::Geode;
	g->setCullingActive(false);
	g->addDrawable(geometry);

	osg::Program * prog = new osg::Program;
	prog->addShader(osg::Shader::readShaderFile(osg::Shader::VERTEX, "jet.vert.glsl"));
	prog->addShader(osg::Shader::readShaderFile(osg::Shader::FRAGMENT, "jet.frag.glsl"));
	g->getOrCreateStateSet()->setAttribute(prog);

	osg::Texture2D * tex = new osg::Texture2D;
	tex->setImage(osgDB::readRefImageFile("Flame.tga"));
	tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR_MIPMAP_LINEAR);
	tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	tex->setWrap(osg::Texture2D::WRAP_R, osg::Texture2D::CLAMP_TO_EDGE);
	tex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_EDGE);
	tex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);
	tex->setResizeNonPowerOfTwoHint(false);

	g->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
	g->getOrCreateStateSet()->addUniform(new osg::Uniform("flametex", 0));
	g->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	osg::BlendFunc * blend = new osg::BlendFunc;
	blend->setFunction(osg::BlendFunc::DST_ALPHA, osg::BlendFunc::ONE);
	blend->setFunction(osg::BlendFunc::DST_COLOR, osg::BlendFunc::ONE);
	g->getOrCreateStateSet()->setAttributeAndModes(blend, osg::StateAttribute::ON);

	osg::Depth * depth = new osg::Depth;
	depth->setWriteMask(false);
	g->getOrCreateStateSet()->setAttributeAndModes(depth, osg::StateAttribute::ON);

	osg::ref_ptr <osgViewer::Viewer> viewer = new osgViewer::Viewer(arguments);
	viewer->setSceneData(g);
	return viewer->run();
}
