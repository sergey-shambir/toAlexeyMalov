#include "EyeCameraInputController.h"
#include "../events/KeyboardEvent.h"
#include "../events/MouseMotionEvent.h"

EyeCameraInputController::EyeCameraInputController(GL::IFirstPersonCamera &reference, float cameraSpeed)
    : m_cameraRef(reference)
    , m_cameraSpeed(cameraSpeed)
{
    m_cameraRef.setTrackingEnabled(true);
    if (m_cameraRef.capturesMouse()) {
        SDL_ShowCursor(SDL_DISABLE);
        m_cameraRef.setTrackingEnabled(true);
    } else {
        m_cameraRef.setTrackingEnabled(false);
    }
}

void EyeCameraInputController::pressedKey(const KeyboardEvent &ev)
{
    vec3d speed = m_cameraRef.speed();
    switch (ev.m_key)
    {
    case SDLK_w:
        speed.x += m_cameraSpeed;
        break;
    case SDLK_a:
        speed.y += m_cameraSpeed;
        break;
    case SDLK_d:
        speed.y -= m_cameraSpeed;
        break;
    case SDLK_s:
        speed.x -= m_cameraSpeed;
        break;
    default:
        break;
    }
    m_cameraRef.setSpeed(speed);
}

void EyeCameraInputController::releasedKey(const KeyboardEvent &ev)
{
    vec3d speed = m_cameraRef.speed();
    switch (ev.m_key)
    {
    case SDLK_w:
        speed.x -= m_cameraSpeed;
        break;
    case SDLK_a:
        speed.y -= m_cameraSpeed;
        break;
    case SDLK_d:
        speed.y += m_cameraSpeed;
        break;
    case SDLK_s:
        speed.x += m_cameraSpeed;
        break;
    default:
        break;
    }
    m_cameraRef.setSpeed(speed);
}

void EyeCameraInputController::movedMouse(const MouseMotionEvent &ev)
{
    std::pair<int, int> delta(ev.m_dx, ev.m_dy);
    if (m_blacklistedMoves.count(delta) > 0) {
        /* Should ignore this event */
        auto removableIter = m_blacklistedMoves.find(delta);
        m_blacklistedMoves.erase(removableIter);
    } else {
        m_cameraRef.movedMouse(ev.m_dx, ev.m_dy);
        safeWarpMouse();
    }
}

void EyeCameraInputController::safeWarpMouse()
{
    if (m_cameraRef.capturesMouse()) {
        std::pair<int, int> center;
        m_cameraRef.getViewport(center.first, center.second);
        center.first /= 2;
        center.second /= 2;
        SDL_WarpMouse(center.first, center.second);

        std::pair<int, int> delta;
        SDL_GetMouseState(&delta.first, &delta.second);
        delta.first = center.first - delta.first;
        delta.second = center.second - delta.second;
        m_blacklistedMoves.insert(delta);
    }
}


void EyeCameraInputController::pressedMouse(const MouseButtonEvent &)
{
    if (!m_cameraRef.capturesMouse())
        m_cameraRef.setTrackingEnabled(true);
}


void EyeCameraInputController::releasedMouse(const MouseButtonEvent &)
{
    if (!m_cameraRef.capturesMouse())
        m_cameraRef.setTrackingEnabled(false);
}
