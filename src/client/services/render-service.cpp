#include "pch.h"
#include "render-service.h"

#include "framework/application.h"

#include <cpu-engine.h>

#include "client-service.h"
#include "messages/fire-message.h"
#include "messages/join-message.h"
#include "objects/coin-entity.h"
#include "objects/player-entity.h"
#include "objects/projectile-entity.h"

RenderService::RenderService() : Service() {
    cpu::Initialize();
    
    ClientService& client = APPLICATION.getService<ClientService>();
    client.AddMessageListener<PlayerJoinedMessage>(this, &RenderService::OnPlayerJoined);
    client.AddMessageListener<FireMessage>(this, &RenderService::OnFireMissile);
    client.AddMessageListener<CoinSpawnMessage>(this, &RenderService::OnCoinSpawned);
}
RenderService::~RenderService() {
    cpu::Uninitialize();

    for (Entity* entity : m_entities) {
        delete entity;
    }
}

void RenderService::destroy(Entity* _entity) {
    m_toRemove.push_back(_entity);
}

void RenderService::OnStartup() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));

    if (m_engine.Create(1280, 720, false, false)) {
        // Bind callbacks
        cpuEngine.GetCallback()->onRender.Set(this, &RenderService::OnRender);
        
        // Window
        m_engine.m_window.Show();

        // Threads
        for ( int i=0 ; i<m_engine.m_threadCount ; i++ )
            m_engine.m_threads[i].Run();

        // Reset
        cpuTime.Reset();
        cpuInput.Reset();
        cpuInput.SetWindow(&m_engine.m_window);
        
        // TEMPORARY
        cpu_camera* camera = cpuEngine.GetCamera();
        camera->transform.pos = XMFLOAT3(0.0f, 20.0f,0.0f);
        camera->transform.AddYPR(0.0f, 3.14f * 0.5f, 0.0f);
        camera->transform.UpdateWorld();
        
        m_font.Create(cpuDevice.GetHeight()<=512 ? 14 : 28);
    }
}
void RenderService::OnUpdate() {
    // Should stop
    if (!m_engine.m_window.Update())
        APPLICATION.shutdown();

    // Update
    m_engine.Update();
    
    // TEMP
    for (auto* entity : m_entities) {
        entity->OnUpdate();
    }

    // Render
    m_engine.Render();
    
    // Add & Remove entities at the end of the frame
    for (auto* entity : m_toAdd) {
        entity->OnSpawned();
        m_entities.push_back(entity);
    }
    m_toAdd.clear();

    for (auto* entity : m_toRemove) {
        entity->OnDestroy();
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
        delete entity;
    }
    m_toRemove.clear();
}
void RenderService::OnShutdown() {
    // Cursor
    SetCursor(nullptr);

    // End
    m_engine.Update_Purge();

    // Threads
    for ( int i=0 ; i< m_engine.m_threadCount ; i++ )
        m_engine.m_threads[i].Stop();

    // Jobs
    m_engine.m_entityJobs.clear();
    m_engine.m_particlePhysicsJobs.clear();
    m_engine.m_particleSpaceJobs.clear();
    m_engine.m_particleRenderJobs.clear();

    // Managers
    m_engine.Update_Purge();
    m_engine.ClearManagers();
}

void RenderService::OnRender(int _pass) {
    switch (_pass) {
    case CPU_PASS_PARTICLE_BEGIN:
        break;
    case CPU_PASS_PARTICLE_END:
        break;
    case CPU_PASS_UI_END:
        {
            // Debug
            cpu_stats& stats = *cpuEngine.GetStats();
            std::string info = CPU_STR(cpuTime.fps) + " fps, ";
            info += CPU_STR(stats.drawnTriangleCount) + " triangles, ";
            info += CPU_STR(stats.clipEntityCount) + " clipped entities\n";
            info += CPU_STR(cpuEngine.GetParticleData()->alive) + " particles, ";
            info += CPU_STR(stats.threadCount) + " threads, ";
            info += CPU_STR(stats.tileCount) + " tiles";
            
            cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth()*0.5f), 10, CPU_TEXT_CENTER);
            break;
        }
    default:
        break;
    }
    
    for (auto* entity : m_entities) {
        entity->OnRender(_pass);
    }
}

void RenderService::OnPlayerJoined(PlayerJoinedMessage* _message, OwnerID _owner) {
    spawn<PlayerEntity>(_message->owner);
}

void RenderService::OnFireMissile(FireMessage* _message, OwnerID _owner) {
    Vector3 position = { _message->posX, _message->posY, _message->posZ };
    XMFLOAT4 rotation = XMFLOAT4{ _message->rotX, _message->rotY, _message->rotZ, _message->rotW };
    spawn<ProjectileEntity>(_message->owner, position, rotation);
}

void RenderService::OnCoinSpawned(CoinSpawnMessage* _message, OwnerID _owner) {
    spawn<CoinEntity>();
}
