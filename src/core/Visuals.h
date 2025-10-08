#pragma once
#include <vector>
#include "Math.h"
#include "../renderer/Interface.h"
#include "../renderer/QuadRenderer.h"

struct DamageIndicator {
    Vector2 position;
    float lifetime;
    float age;
    float damage;
    Vector4 color;
    float riseSpeed;

    DamageIndicator(Vector2 pos, float dmg)
        : position(pos),
        lifetime(1.0f), // 1 second duration
        age(0.0f),
        damage(dmg),
        color(1.0f, 0.2f, 0.2f, 1.0f),
        riseSpeed(40.0f) {
    }
};


class DamageIndicatorSystem {
private:
    static inline QuadRenderer* m_quadrender;
    static inline std::vector<DamageIndicator> m_indicators;

public:
    static void setRender(QuadRenderer* qRender);
    static void spawn(const Vector2& position, float dmg);
    static void update(float dt);
    static void render(Interface& ui, int fontIndex);
};
