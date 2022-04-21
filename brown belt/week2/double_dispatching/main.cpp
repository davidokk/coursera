#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

template <typename T>
struct Collider : GameObject {
  bool Collide(const GameObject& that) const override { return that.CollideWith(static_cast<const T&>(*this)); }
};

class Unit : public Collider<Unit> {
 public:
  explicit Unit(geo2d::Point position) : object(position) {}

  geo2d::Point GetObject() const;

  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;
 private:
  geo2d::Point object;
};

class Building : public Collider<Building> {
 public:
  explicit Building(geo2d::Rectangle geometry) : object(geometry) {}

  geo2d::Rectangle GetObject() const;

  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;
 private:
  geo2d::Rectangle object;
};

class Tower : public Collider<Tower> {
 public:
  explicit Tower(geo2d::Circle geometry) : object(geometry) {}

  geo2d::Circle GetObject() const;

  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;
 private:
  geo2d::Circle object;
};

class Fence : public Collider<Fence> {
 public:
  explicit Fence(geo2d::Segment geometry) : object(geometry) {}

  geo2d::Segment GetObject() const;

  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;
 private:
  geo2d::Segment object;
};

geo2d::Point Unit::GetObject() const { return object; }
geo2d::Rectangle Building::GetObject() const { return object; }
geo2d::Circle Tower::GetObject() const { return object; }
geo2d::Segment Fence::GetObject() const { return object; }

#define COLLIDE_WITH_DEFINITION(Class, Other) bool Class::CollideWith(const Other& that) const { return geo2d::Collide(object, that.GetObject()); }

#define COLLIDE_WITH_OTHER(Class) \
  COLLIDE_WITH_DEFINITION(Class, Unit) \
  COLLIDE_WITH_DEFINITION(Class, Building) \
  COLLIDE_WITH_DEFINITION(Class, Tower) \
  COLLIDE_WITH_DEFINITION(Class, Fence)

COLLIDE_WITH_OTHER(Unit)
COLLIDE_WITH_OTHER(Building)
COLLIDE_WITH_OTHER(Tower)
COLLIDE_WITH_OTHER(Fence)

bool Collide(const GameObject& first, const GameObject& second) { return first.Collide(second); }

void TestAddingNewObjectOnMap() {
  // Р®РЅРёС‚-С‚РµСЃС‚ РјРѕРґРµР»РёСЂСѓРµС‚ СЃРёС‚СѓР°С†РёСЋ, РєРѕРіРґР° РЅР° РёРіСЂРѕРІРѕР№ РєР°СЂС‚Рµ СѓР¶Рµ РµСЃС‚СЊ РєР°РєРёРµ-С‚Рѕ РѕР±СЉРµРєС‚С‹,
  // Рё РјС‹ С…РѕС‚РёРј РґРѕР±Р°РІРёС‚СЊ РЅР° РЅРµС‘ РЅРѕРІС‹Р№, РЅР°РїСЂРёРјРµСЂ, РїРѕСЃС‚СЂРѕРёС‚СЊ РЅРѕРІРѕРµ Р·РґР°РЅРёРµ РёР»Рё Р±Р°С€РЅСЋ.
  // РњС‹ РјРѕР¶РµРј РµРіРѕ РґРѕР±Р°РІРёС‚СЊ, С‚РѕР»СЊРєРѕ РµСЃР»Рё РѕРЅ РЅРµ РїРµСЂРµСЃРµРєР°РµС‚СЃСЏ РЅРё СЃ РѕРґРЅРёРј РёР· СЃСѓС‰РµСЃС‚РІСѓСЋС‰РёС….
  using namespace geo2d;

  const vector<shared_ptr<GameObject>> game_map = {
      make_shared<Unit>(Point{3, 3}),
      make_shared<Unit>(Point{5, 5}),
      make_shared<Unit>(Point{3, 7}),
      make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
      make_shared<Tower>(Circle{Point{9, 4}, 1}),
      make_shared<Tower>(Circle{Point{10, 7}, 1}),
      make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
  };

  for (size_t i = 0; i < game_map.size(); ++i) {
    Assert(
        Collide(*game_map[i], *game_map[i]),
        "An object doesn't collide with itself: " + to_string(i)
    );

    for (size_t j = 0; j < i; ++j) {
      Assert(
          !Collide(*game_map[i], *game_map[j]),
          "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
      );
    }
  }

  auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT( Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT( Collide(*new_warehouse, *game_map[3]));
  ASSERT( Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT( Collide(*new_defense_tower, *game_map[3]));
  ASSERT( Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
  TestRunner tr;
  //RUN_TEST(tr, TestAddingNewObjectOnMap);
  return 0;
}