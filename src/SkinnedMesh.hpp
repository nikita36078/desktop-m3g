#ifndef __SKINNED_MESH_HPP__
#define __SKINNED_MESH_HPP__

#include "Mesh.hpp"
#include <iosfwd>
#include <vector>

#include "BoneIndex.hpp"


namespace m3g {

  class Group;
  class VertexBuffer;
  class IndexBuffer;
  
  


  /**
   * @~English  A scene graph node that represents a skeletally animated polygon mesh.
   * @~Japanese スケレタルアニメーテッドポリゴンメッシュを表すシーングラフノード.
   */
  class SkinnedMesh : public Mesh
  {
  public:

    /**
     * @~English  Constructs a new SkinnedMesh witdh the given vertices, submeshes and skeleton.
     * @~Japanese 指定された頂点、サブメッシュ、スケルトンを持つSkinnedMeshを新しく作成する.
     */
    SkinnedMesh (VertexBuffer* vertices, 
                 int           num_submesh,
                 IndexBuffer** submeshes,
                 Appearance**  apperarances,
                 Group*        skelton);

    /**
     * @~English  Constructs a new SkinnedMesh witdh the given vertices, submeshes and skeleton.
     * @~Japanese 指定された頂点、サブメッシュ、スケルトンを持つSkinnedMeshを新しく作成する.
     */
    SkinnedMesh (VertexBuffer* vertices, 
                 IndexBuffer*  submeshes, 
                 Appearance*   apperarances,
                 Group*        skelton);

    /**
     * @~English  Destructs this object.
     * @~Japanese このオブジェクトを削除するデストラクタ.
     */
    virtual ~SkinnedMesh ();

    /**
     * @~English  Creates a duplicate of this Object3D. 
     * @~Japanese このオブジェクトの複製の作成.
     */
    SkinnedMesh* duplicate () const;

    virtual int animate (int world_time);

    /**
     * @~English  Associates a weighted transformation, or "bone", with a range of vertices int this SkinnedMesh.
     * @~Japanese このスキンメッシュの頂点配列に対してウェイト付き変形(ボーン)を関連づける.
     */
    void addTransform (Node* bone, int weight, int first_vertex, int num_vertices);

    /**
     * @~English  Returns the at-rest tarnsformation (from local coordinate to bone coordinate)  for a bone node.
     * @~Japanese ボーンノードの基本姿勢の変形(ローカル座標系からボーン座標系への変換行列)を返す.
     */
    void getBoneTransform (Node* bone, Transform* transform) const;

    /**
     * @~English Returns the number of vertices influenced by the given bone, filling in the vertices and their weights to given arrays. 
     * @~Japanese 指定されたボーンに影響を受ける頂点の数を返す。与えられた配列に頂点のインデックスとウェイト値が入る.
     */
    int getBoneVertices (Node* bone, int* vertex_indices, float* weights) const;

    /**
     * @~English  Returns the skeleton Group of this SkinnedMesh.
     * @~Japanese このスキンメッシュのスケルトンGroupを返す.
     */
    Group* getSkeleton () const;

    /**
     * @~English  Print out information of this class, for debug only.
     * @~Japanese このSkinnedMeshクラスの情報を表示する。デバッグ用.
     */
    virtual std::ostream& print (std::ostream& out) const;


    /**
     * @~English  Render this object.
     * @~Japanese このSkinnedMeshをレンダリングする内部使用の関数.
     */
    virtual void render (RenderState& state) const;


  private:

    /**
     * @~English   Register bone and retriev it's index.
     * @~Japanese  ボーンの登録とそのインデックスの取得.
     */
    int addBoneIndex (Node* bone);

    /**
     * @~English   Retriev bone index.
     * @~Japanese  ボーンインデックスの取得.
     */
    int getBoneIndex (Node* bone) const;

    
    void updateSkinnedVertices ();


  private:
    Group*        skeleton;
    VertexBuffer* skinned_vertices;

    std::vector<std::vector<BoneIndex> > bone_indices;
    std::vector<BindPose>                bind_poses;
  };


} // namespace m3g {

std::ostream& operator<< (std::ostream& out, const m3g::SkinnedMesh& mesh);


#endif
