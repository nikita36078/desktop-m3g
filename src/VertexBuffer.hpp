#ifndef __VERTEX_BUFFER_HPP__
#define __VERTEX_BUFFER_HPP__

#include <GL/glew.h>
#include "Object3D.hpp"
#include "m3gdef.hpp"
#include <iosfwd>

namespace m3g {

  class VertexArray;

  /**
   * @~English  VertexBuffer holds references to VertexArrays that 
   *            contain the positions, colors, normals, and 
   *            texture coordinates for a set of vertices.
   * @~Japanese VertexBufferは頂点位置、法線、色、テクスチャー座標を保持する
   *            VertexArrayへのポインターを持つホルダークラス.
   */
  class VertexBuffer : public Object3D
  {
    friend class Mesh;  // for render

  public:
    /**
     * @~English  Creates an empty VertexBuffer with default values.
     * @~Japanese デフォルトの値のVertexBufferオブジェクトを作成.
     */
    VertexBuffer ();

    /**
     * @~English  Destructs this object.
     * @~Japanese このオブジェクトを削除するデストラクタ.
     */
    virtual ~VertexBuffer ();

    /**
     * @~English  Creates a duplicate of this Object3D. 
     * @~Japanese このオブジェクトの複製の作成.
     */
    VertexBuffer* duplicate () const;

    virtual int animate (int world_time);

    virtual void addAnimationTrack (AnimationTrack* animation_track);


    /**
     * @~English  Gets the current color array.
     * @~Japanese カレントのカラー配列の取得.
     */
    VertexArray* getColors () const;

    /**
     * @~English  Retrieves the default color of this VertexBuffer.
     * @~Japanese このVertexBufferのデフォルトカラーを取得.
     */
    int getDefaultColor () const;

    /**
     * @~English  Gets the current normal vector array.
     * @~Japanese カレントの法線ベクトルの配列の取得.
     */
    VertexArray* getNormals () const;

    /**
     * @~English  Returns the current vertex position array.
     * @~Japanese カレントの頂点座標の配列の取得.
     */
    VertexArray* getPositions (float* scale_bias) const;

    /**
     * @~English  Gets the current texture coordinate array for the specified texture unit.
     * @~Japanese カレントのテクスチャー座標の配列の取得.
     */
    VertexArray* getTexCoords (int index, float* scale_bias) const;

    /**
     * @~English  Sets the per-vertex color for this VertexBuffer.
     * @~Japanese このVertexBufferに頂点カラーを設定する.
     */
    void setColors (VertexArray* colors);

    /**
     * @~English  Sets the color to use in absence of per-vetex colors.
     * @~Japanese 頂点カラーが設定されていないときのカラーを設定する.
     */
    void setDefaultColor (int argb);

    /**
     * @~English  Sets the normal vectors for this VertexBuffer.
     * @~Japanese このVertexBufferに頂点法線を設定する.
     */
    void setNormals (VertexArray* normals);

    /**
     * @~English  Sets the vertex positions for this VertexBuffer.
     * @~Japanese このVertexBufferに頂点座標を設定する.
     */
    void setPositions (VertexArray* positios, float scale, float* bias);

    /**
     * @~English  Sets the texture coordinates for the specified textureing unit.
     * @~Japanese このVertexBufferにテクスチャー座標を設定する.
     */
    void setTexCoords (int index, VertexArray* tex_coords, float scale, float* bias);

    /**
     * @~English  Print out information of this object, for debug only.
     * @~Japanese このVertexBufferクラスの情報を表示する。デバッグ用.
     */
    virtual std::ostream& print (std::ostream& out) const;


    /**
     * @~English  Render this object, for inner use.
     * @~Japanese このオブジェクトをレンダリングする内部使用の関数.
     */
    virtual void render (RenderState& state) const;


  private:
    VertexArray* positions;
    VertexArray* normals;
    VertexArray* colors;
    VertexArray* tex_coords[MAX_TEXTURE_UNITS];

    float        positions_scale;
    float        positions_bias[3];
    float        tex_coords_scale[MAX_TEXTURE_UNITS];
    float        tex_coords_bias[MAX_TEXTURE_UNITS][3];

    int          default_color;

  private:
    GLuint       vbo_positions;
    GLuint       vbo_colors;
    GLuint       vbo_normals;
    GLuint       vbo_texcoords[MAX_TEXTURE_UNITS];

  };


} // namespace m3g {

std::ostream& operator<< (std::ostream& out, const m3g::VertexBuffer& vbuf);


#endif

