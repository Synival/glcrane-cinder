#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ObjLoader.h"
#include "cinder/CinderMath.h"
#include "cinder/Color.h"
#include "cinder/ImageIo.h"

#include "../include/GLCrane.hpp"

using namespace ci;
using namespace ci::app;
using namespace gl;
using namespace std;

void GLCrane::setup ()
{
   // Load our assets.
   try {
      // Load shaders.
      mProgTexture = GlslProg::create (GlslProg::Format()
         .vertex   (loadAsset ("texture.vert"))
         .fragment (loadAsset ("texture.frag"))
      );

      // Load textures.
      mTexPaper  = Texture2d::create (loadImage (loadAsset ("paper.jpg")));
      mTexShadow = Texture2d::create (loadImage (loadAsset ("shadow.png")));

      // Load our crane object.
      ObjLoader loader (loadAsset ("crane.obj"));
      mBatchCrane = gl::Batch::create (loader.groupName ("body"),
         mProgTexture);
      mBatchShadow = gl::Batch::create (loader.groupName ("shadow"),
         mProgTexture);

      // Set up our OpenGL context.
      gl::enableDepthWrite ();
      gl::enableDepthRead ();
   }
   // Log and bail if nothing works.
   catch (ci::Exception ex) {
      cerr << ex.what() << endl;
      quit ();
   }
}

void GLCrane::update ()
{
   // Get frame delta.
   mTotalTime = getElapsedSeconds ();
   if (mLastFrame == 0.00f)
      mLastFrame = mTotalTime;
   else {
      float currentFrame = mTotalTime;
      mFrameDelta = currentFrame - mLastFrame;
      mLastFrame = currentFrame;
   }

   // Decellerate very slowly when not holding the mouse down.
   // TODO: don't separate components - appy to the entire vector.
   if (!mMouseDown) {
      floatApproach (mMouseVelX, 0.00f, 1.00f, mFrameDelta);
      floatApproach (mMouseVelY, 0.00f, 1.00f, mFrameDelta);
   }
   // Otherwise, decellerate very quickly.
   else {
      // Skip frames in which we called mouseDrag().
      if (mMouseUpdated)
         mMouseUpdated = false;
      else {
         floatApproach (mMouseVelX, 0.00f, 30.00f, mFrameDelta);
         floatApproach (mMouseVelY, 0.00f, 30.00f, mFrameDelta);
      }
   }
}

void GLCrane::draw ()
{
   int width  = getWindowWidth ();
   int height = getWindowHeight ();
   int max = std::min (width, height);

   // Record our scene dimensions.
   mSceneWidth  = (float) width  / (float) max;
   mSceneHeight = (float) height / (float) max;

   // Set up perspective matrix at 45 degree angle with (-1, -1) to (1, 1)
   // box viewport.  Origin is lower-left.
   gl::setMatricesWindowPersp (width, height, 45.00f,
      1.00f, 1000.00f * max, false);
   gl::translate (width / 2, height / 2, 0.00f);
   gl::scale (max / 2, max / 2, max / 2);

   // Set up the position for our rotating crane.
   gl::translate (0.00f, -0.25f, -1.50f);
   gl::rotate (toRadians (mRotateX), 1.00f, 0.00f, 0.00f);
   gl::rotate (toRadians (mRotateY), 0.00f, 1.00f, 0.00f);

   // Start drawing! Clear window.
   gl::clear (Color::gray (0.25f));

   // Color!
   vec3 hsv (mHue, mSat, mVal);
   Colorf color = hsvToRgb (hsv);
   gl::color (color.r, color.g, color.b, mAlpha);

   // Draw crane with our 'texture' shader.
   gl::ScopedGlslProg glslProg (mProgTexture);
   mProgTexture->uniform ("uTime", mTotalTime);

   // Draw paper crane and its shadow.
   mTexPaper->bind ();
   mBatchCrane->draw ();
   mTexShadow->bind ();
   mBatchShadow->draw ();

   // Rotate crane based on mouse movements.
   mRotateX += mMouseVelY;
   mRotateY += mMouseVelX;

   // Animations.
   mHue = fmodf (mHue + 0.25f * (mFrameDelta / 6.00f), 1.00f);
}

void GLCrane::mouseUp (MouseEvent event)
{
   mMouseDown = false;
}

void GLCrane::mouseDown (MouseEvent event)
{
   mMouseDown = true;
   getMousePos (event.getPos(), mMouseLastX, mMouseLastY);
}

void GLCrane::mouseDrag (MouseEvent event)
{
   // Record that we dragged the mouse this frame.
   mMouseUpdated = true;

   // Get current mouse position and push our torque.
   float mouseX, mouseY;
   getMousePos (event.getPos(), mouseX, mouseY);
   floatApproach (mMouseVelX, mouseX - mMouseLastX, 30.00f, mFrameDelta);
   floatApproach (mMouseVelY, mouseY - mMouseLastY, 30.00f, mFrameDelta);

   // Remember the last mouse position.
   mMouseLastX = mouseX;
   mMouseLastY = mouseY;
}

void GLCrane::initSettings (GLCrane::Settings *settings)
{
   // Set window to 75% width/height.
   DisplayRef screen = Display::getMainDisplay ();
   int width  = screen->getWidth(),
       height = screen->getHeight();
   settings->setWindowSize (width * 0.75f, height * 0.75f);

   // One finger at a time, please!
   settings->setMultiTouchEnabled (false);
}

void GLCrane::floatApproach (float &val, float target, float speed, float t)
{
        if (val < target) val = std::min (val + speed * t, target);
   else if (val > target) val = std::max (val - speed * t, target);
}

void GLCrane::getMousePos (ivec2 mousePos, float &x_out, float &y_out)
{
   // Write mouse position out in terms of degrees.
   float width  = getWindowWidth(),
         height = getWindowHeight();
   x_out = (float) mousePos.x / width  * mSceneWidth  * 360.00f;
   y_out = (float) mousePos.y / height * mSceneHeight * 360.00f;
}

// Register our app.
CINDER_APP (GLCrane, RendererGl, GLCrane::initSettings)
