#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Texture.h"
#include "cinder/ObjLoader.h"

using namespace ci;
using namespace ci::app;
using namespace gl;

class GLCrane : public ci::app::App {
   public:
      // API functions.
      void setup()  override;
      void update() override;
      void draw()   override;
      void mouseUp   (MouseEvent event) override;
      void mouseDown (MouseEvent event) override;
      void mouseDrag (MouseEvent event) override;

      // Our own stuff.
      static void initSettings (GLCrane::Settings *settings);
      static void floatApproach (float &val, float target,
         float speed, float t);
      void getMousePos (ivec2 mousePos, float &x_out, float &y_out);

   private:
      // Member variables.
      GlslProgRef  mProgTexture;
      BatchRef     mBatchCrane, mBatchShadow;
      mat4         mModelViewProjection;
      float        mSceneWidth, mSceneHeight;
      float        mLastFrame, mFrameDelta, mRotateX{15.00f}, mRotateY{30.00f};
      float        mHue{0.00f}, mSat{0.00f}, mVal{1.00f}, mAlpha{1.00f};
      float        mMouseVelX, mMouseVelY;
      float        mMouseLastX, mMouseLastY;
      float        mTotalTime;
      Texture2dRef mTexPaper, mTexShadow;
      bool         mMouseDown{false}, mMouseUpdated{false};
};
