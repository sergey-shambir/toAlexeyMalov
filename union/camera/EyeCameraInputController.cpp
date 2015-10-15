#include "EyeCameraInputController.h"
#include "../events/KeyboardEvent.h"
#include "../events/MouseMotionEvent.h"
#include "../app/Window.h"

EyeCameraInputController::EyeCameraInputController(const Window &window, GL::IFirstPersonCamera &reference, float cameraSpeed)
    : m_window(window)
    , m_cameraRef(reference)
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
    switch (ev.m_key)
    {
    case SDLK_w:
        m_moveState.keyW = true;
        break;
    case SDLK_a:
        m_moveState.keyA = true;
        break;
    case SDLK_d:
        m_moveState.keyD = true;
        break;
    case SDLK_s:
        m_moveState.keyS = true;
        break;
    case SDLK_UP:
        m_moveState.forward = true;
        break;
    case SDLK_LEFT:
        m_moveState.left = true;
        break;
    case SDLK_RIGHT:
        m_moveState.right = true;
        break;
    case SDLK_DOWN:
        m_moveState.backward = true;
        break;
    default:
        break;
    }
    updateCameraSpeed();
}

void EyeCameraInputController::releasedKey(const KeyboardEvent &ev)
{
    switch (ev.m_key)
    {
    case SDLK_w:
        m_moveState.keyW = false;
        break;
    case SDLK_a:
        m_moveState.keyA = false;
        break;
    case SDLK_d:
        m_moveState.keyD = false;
        break;
    case SDLK_s:
        m_moveState.keyS = false;
        break;
    case SDLK_UP:
        m_moveState.forward = false;
        break;
    case SDLK_LEFT:
        m_moveState.left = false;
        break;
    case SDLK_RIGHT:
        m_moveState.right = false;
        break;
    case SDLK_DOWN:
        m_moveState.backward = false;
        break;
    default:
        break;
    }
    updateCameraSpeed();
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
        m_window.warpMouse(center.first, center.second);

        std::pair<int, int> delta;
        SDL_GetMouseState(&delta.first, &delta.second);
        delta.first = center.first - delta.first;
        delta.second = center.second - delta.second;
        m_blacklistedMoves.insert(delta);
    }
}

void EyeCameraInputController::updateCameraSpeed()
{
    vec3d speed(0, 0, 0);
    if (m_moveState.forward || m_moveState.keyW) {
        speed.x += m_cameraSpeed;
    }
    if (m_moveState.backward || m_moveState.keyS) {
        speed.x -= m_cameraSpeed;
    }
    if (m_moveState.left || m_moveState.keyA) {
        speed.y += m_cameraSpeed;
    }
    if (m_moveState.right || m_moveState.keyD) {
        speed.y -= m_cameraSpeed;
    }
    m_cameraRef.setSpeed(speed);
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
