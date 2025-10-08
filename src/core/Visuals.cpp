#include "Visuals.h"


void DamageIndicatorSystem::spawn(const Vector2& position, float dmg) {
    m_indicators.emplace_back(position, dmg);
}

void DamageIndicatorSystem::update(float dt) {
    for (auto it = m_indicators.begin(); it != m_indicators.end();) {
        it->age += dt;
        it->position.y += it->riseSpeed * dt; // float upward

        // fade out near end
        float lifeRatio = 1.0f - (it->age / it->lifetime);
        it->color.w = glm::clamp(lifeRatio, 0.0f, 1.0f);

        if (it->age >= it->lifetime)
            it = m_indicators.erase(it);
        else
            ++it;
    }
}
void DamageIndicatorSystem::setRender(QuadRenderer* qRender) { m_quadrender = qRender; }
void DamageIndicatorSystem::render(Interface& ui, int fontIndex) {
    for (auto& ind : m_indicators) {
        std::string dmgText = std::to_string((int)ind.damage);
        ui.renderText(dmgText, fontIndex, m_quadrender->worldToScreen(ind.position), 1.2f, ind.color, 1.2f);
    }
}
