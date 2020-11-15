//
// Created by raykuzu on 30/09/2020.
//

// #ifndef _WIN32
//     #include <csignal>
// #endif
// #include "PhysicsModule.hpp"
// #include "Shell.hpp"
// #include "Scene.hpp"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #ifndef _WIN32
// void sigHandler(__attribute__((unused)) int signum) {
//     ArcLogger::info("Game engine teardown ...");
//     ArcLogger::unsetLogLevel(ArcLogger::ERROR);
//     Shell::running = false;
// }
// #endif

// int main(__attribute__((unused)) int ac, __attribute__((unused)) char **av) {
//     ArcLogger::setBinaryName("GE3D");
//     ArcLogger::setOutputDir("./log");
//     ArcLogger::setLogLevel(ArcLogger::TRACE);
//     ArcLogger::setLogLevel(ArcLogger::DEBUG);
    

//     #ifndef _WIN32
//     struct sigaction sgt {};

//     sgt.sa_handler = &sigHandler;
//     sgt.sa_flags = 0;
//     sgt.sa_mask = {};
//     sgt.sa_restorer = nullptr;

//     sigaction(SIGHUP, &sgt, nullptr);
//     sigaction(SIGINT, &sgt, nullptr);
//     sigaction(SIGTERM, &sgt, nullptr);
//     //signal(SIGPIPE, SIG_IGN); voir si c'est utile
//     #endif

//     Shell shell;

//     Scene sc("scene1");
//     std::vector<sharedGO> list;
//     sharedGO go(new GameObject(0x01 | 0x02));
//     collider_t *tmp = new collider_t(Collider::SPHERE, new sphereCollider_t(EngineMath::Vector3(0,0,0), 1.1));
//     go->getComponent<collider_t*>(Component::COLLIDER)->addCollider(Collider::SPHERE);

//     go->setComponent(Component::COLLIDER, tmp);
//     go.get()->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(0,0,0), EngineMath::Vector3(1,0,0)));
//     list.push_back(go);
//     sc.addGameObject(go);

//     EngineMath::m3_t matrix(1, 8,7,3,9,5,1,0,4);

//     matrix = EngineMath::Minor(matrix);

//     EngineMath::display(matrix);
//     EngineMath::m4_t mtrix(12.0f,32.0f,19.0f,36.0f,42.0f,12.0f,77.0f,53.0f,87.0f,99.0f,91.0f,63.0f,51.0f,36.0f,27.0f,7.0f);
//     EngineMath::display(mtrix);
//     mtrix = EngineMath::Inverse(mtrix);
//     EngineMath::display(mtrix);
//     go = sharedGO(new GameObject(0x01 | 0x02));
//     go->getComponent<collider_t*>(Component::COLLIDER)->addCollider(Collider::SPHERE);
//     collider_t *tmp2 = new collider_t(Collider::SPHERE, new sphereCollider_t(EngineMath::Vector3(4,0,0), 1.1));
//     tmp->addCollider(Collider::SPHERE);

//     go->setComponent(Component::COLLIDER, tmp2);
//     go->setComponent(Component::TRANSFORM, new transform_t(EngineMath::Vector3(4,0,0), EngineMath::Vector3(0,0,0)));
//     list.push_back(go);
//     sc.addGameObject(go);

    
//     PhysicsModule phm = PhysicsModule();

//     for (int i = 0; i < 5; i++) {
//         phm.update(sc);
//         phm.display(sc);
//     }

//     glm::mat4 glmMtrx = glm::lookAt(glm::vec3(2.0f + (1.0f * 2), 0.0f, 2.0f), glm::vec3(-1.0f + (1.0f * 2), 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//     EngineMath::m4_t engineMtrx = EngineMath::LookAt(EngineMath::Vector3(2.0f + (1.0f * 2), 0.0f, 2.0f), EngineMath::Vector3(-1.0f + (1.0f * 2), 0.0f, 0.0f), EngineMath::Vector3(0.0f, 0.0f, 1.0f));
//         std::cout << std::setprecision(4);

//     std::cout << "GLM MTRX" << std::endl;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             std::cout << glmMtrx[i][j] << "\t" ;
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "ENGINE MTRX" << std::endl;
//     EngineMath::display(engineMtrx);

//     glm::mat4 glmMtrxProj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 10.0f);
//     EngineMath::m4_t engineMtrxProj = EngineMath::Projection(45.0f, 1920.0f / 1080.0f, 0.1f, 10.0f);

//     std::cout << "GLM MTRXproj" << std::endl;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             std::cout << glmMtrxProj[i][j] << "\t" ;
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "ENGINE MTRXproj" << std::endl;
//     EngineMath::display(engineMtrxProj);

//     glm::mat4 glmMtrxRot = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//     glm::mat4 glmMtrxRotEul = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
//     EngineMath::m4_t engineMtrxRot = EngineMath::AxisAngle(EngineMath::Vector3(0.0f, 0.0f, 1.0f), 40);

//     std::cout << "GLM MTRXRot" << std::endl;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             std::cout << glmMtrxRot[i][j] << "\t" ;
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "ENGINE MTRXRot" << std::endl;
//     EngineMath::display(engineMtrxRot);

//     EngineMath::Vector3 v(1,2,3);
//     std::cout << v.getDisplayInfo() << std::endl;
    
//     EngineMath::Vector3 mv(-v);
//     std::cout << v.getDisplayInfo() << std::endl;
    


//     phm.testModule();

//     // shell.run();
//     return (0);
// }
