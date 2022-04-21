#include "Common.h"
#include "Textures.h"



using namespace std;

bool PointInTexture(Point p, const shared_ptr<ITexture>& texture) {
  if (!texture)
    return false;
  return p.x < texture->GetSize().width && p.y < texture->GetSize().height;
}

class Rectangle : public IShape {
 public:
  unique_ptr<IShape> Clone() const override {
    return make_unique<Rectangle>(Rectangle(*this));
  }

  void SetPosition(Point pos) override { position = pos; }
  Point GetPosition() const override { return position; }

  void SetSize(Size s) override { size = s; }
  Size GetSize() const override { return size; }

  void SetTexture(shared_ptr<ITexture> t) override { texture = t; }
  ITexture* GetTexture() const override { return texture.get(); }

  void Draw(Image& image) const override {

    int h = image.size();
    int w = image.empty()? 0 : image[0].size();
    for (int i = 0; i < size.height; i++)
      for (int j = 0; j < size.width; j++) {
        if (i + position.y >= h || j + position.x >= w)
          break;
        image[i + position.y][j + position.x] = PointInTexture({j, i}, texture) ? texture->GetImage()[i][j] : '.';
      }
  }

 private:
  Size size;
  Point position;
  shared_ptr<ITexture> texture;
};

class Ellipse : public IShape {
 public:
  unique_ptr<IShape> Clone() const override {
    return make_unique<Ellipse>(Ellipse(*this));
  }

  void SetPosition(Point pos) override { position = pos; }
  Point GetPosition() const override { return position; }

  void SetSize(Size s) override { size = s; }
  Size GetSize() const override { return size; }

  void SetTexture(shared_ptr<ITexture> t) override { texture = t; }
  ITexture* GetTexture() const override { return texture.get(); }

  void Draw(Image& image) const override {
    int h = image.size();
    int w = image.empty()? 0 : image[0].size();
    for (int i = 0; i < size.height; i++)
      for (int j = 0; j < size.width; j++) {
        if (i + position.y >= h || j + position.x >= w)
          break;
        if (IsPointInEllipse({j, i}, size))
          image[i + position.y][j + position.x] = PointInTexture({j, i}, texture) ? texture->GetImage()[i][j] : '.';
      }
  }

 private:
  Size size;
  Point position;
  shared_ptr<ITexture> texture;
};

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  if (shape_type == ShapeType::Rectangle)
    return make_unique<Rectangle>();
  return make_unique<Ellipse>();
}