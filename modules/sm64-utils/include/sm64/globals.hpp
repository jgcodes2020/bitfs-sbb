#ifndef SM64_GLOBALS_HPP_INCLUDED
#define SM64_GLOBALS_HPP_INCLUDED

#include <type_traits>
#include "sm64/camera.hpp"
#include "sm64/constants.hpp"
#include "sm64/types.hpp"

namespace sm64 {
  namespace details {
    template <class T, class = void>
    struct is_sized_impl : std::false_type {};
    template <class T>
    struct is_sized_impl<T, std::void_t<decltype(sizeof(T))>> : std::true_type {
    };

    // Checks if a type can be dereferenced, i.e. sizeof(T) is valid.
    template <class T>
    constexpr bool is_sized_v = is_sized_impl<T>::value;
  }  // namespace details

  // Special tag type for valid sized global objects.
  // These cannot be created by the user; they *must* be declared in the
  // `globals` class below.
  template <class T>
  class sized_global final {
    friend class globals;
    static_assert(details::is_sized_v<T>, "sized_global requires a sized type");

  public:
    const char* name() const { return m_name; }

  private:
    constexpr sized_global(const char* name) : m_name(name) {}
    const char* m_name;
  };

  // Special tag type for valid opaque global objects.
  // These cannot be created by the user; they *must* be declared in the
  // `globals` class below.
  template <class T>
  class ptr_global final {
    friend class globals;
    static_assert(std::is_pointer_v<T>, "ptr_global requires a pointer type");

  public:
    const char* name() const { return m_name; }

  private:
    constexpr ptr_global(const char* name) : m_name(name) {}
    const char* m_name;
  };

  // A directory of usable globals from libsm64.
  //
  struct globals {
    globals() = delete;
#define SM64_GLOBAL(type, name)              \
  static constexpr sized_global<type> name { \
    #name                                    \
  }
#define SM64_PTR(type, name)               \
  static constexpr ptr_global<type> name { \
    #name                                  \
  }

#pragma region Useful globals
    // Mario's state.
    SM64_GLOBAL(MarioState[1], gMarioStates);
    // The state of the controllers.
    SM64_GLOBAL(OSContPad[4], gControllerPads);
    // The state of the camera.
    SM64_GLOBAL(Camera*, gCamera);
    // The pool to which all objects are allocated.
    SM64_GLOBAL(Object[OBJECT_POOL_CAPACITY], gObjectPool);
    // The index of the current stage.
    SM64_GLOBAL(s16, gCurrLevelNum);
#pragma endregion

#pragma region Behavior scripts
    SM64_PTR(const BehaviorScript*, bhvStarDoor);
    SM64_PTR(const BehaviorScript*, bhvMrI);
    SM64_PTR(const BehaviorScript*, bhvMrIBody);
    SM64_PTR(const BehaviorScript*, bhvMrIParticle);
    SM64_PTR(const BehaviorScript*, bhvPurpleParticle);
    SM64_PTR(const BehaviorScript*, bhvGiantPole);
    SM64_PTR(const BehaviorScript*, bhvPoleGrabbing);
    SM64_PTR(const BehaviorScript*, bhvTHIHugeIslandTop);
    SM64_PTR(const BehaviorScript*, bhvTHITinyIslandTop);
    SM64_PTR(const BehaviorScript*, bhvCapSwitchBase);
    SM64_PTR(const BehaviorScript*, bhvCapSwitch);
    SM64_PTR(const BehaviorScript*, bhvKingBobomb);
    SM64_PTR(const BehaviorScript*, bhvBobombAnchorMario);
    SM64_PTR(const BehaviorScript*, bhvBetaChestBottom);
    SM64_PTR(const BehaviorScript*, bhvBetaChestLid);
    SM64_PTR(const BehaviorScript*, bhvBubbleParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvBubbleMaybe);
    SM64_PTR(const BehaviorScript*, bhvSmallWaterWave);
    SM64_PTR(const BehaviorScript*, bhvSmallWaterWave398);
    SM64_PTR(const BehaviorScript*, bhvWaterAirBubble);
    SM64_PTR(const BehaviorScript*, bhvSmallParticle);
    SM64_PTR(const BehaviorScript*, bhvPlungeBubble);
    SM64_PTR(const BehaviorScript*, bhvSmallParticleSnow);
    SM64_PTR(const BehaviorScript*, bhvSmallParticleBubbles);
    SM64_PTR(const BehaviorScript*, bhvFishGroup);
    SM64_PTR(const BehaviorScript*, bhvCannon);
    SM64_PTR(const BehaviorScript*, bhvCannonBarrel);
    SM64_PTR(const BehaviorScript*, bhvCannonBaseUnused);
    SM64_PTR(const BehaviorScript*, bhvChuckya);
    SM64_PTR(const BehaviorScript*, bhvChuckyaAnchorMario);
    SM64_PTR(const BehaviorScript*, bhvUnused05A8);
    SM64_PTR(const BehaviorScript*, bhvRotatingPlatform);
    SM64_PTR(const BehaviorScript*, bhvTower);
    SM64_PTR(const BehaviorScript*, bhvBulletBillCannon);
    SM64_PTR(const BehaviorScript*, bhvWFBreakableWallRight);
    SM64_PTR(const BehaviorScript*, bhvWFBreakableWallLeft);
    SM64_PTR(const BehaviorScript*, bhvKickableBoard);
    SM64_PTR(const BehaviorScript*, bhvTowerDoor);
    SM64_PTR(const BehaviorScript*, bhvRotatingCounterClockwise);
    SM64_PTR(const BehaviorScript*, bhvWFRotatingWoodenPlatform);
    SM64_PTR(const BehaviorScript*, bhvKoopaShellUnderwater);
    SM64_PTR(const BehaviorScript*, bhvExitPodiumWarp);
    SM64_PTR(const BehaviorScript*, bhvFadingWarp);
    SM64_PTR(const BehaviorScript*, bhvWarp);
    SM64_PTR(const BehaviorScript*, bhvWarpPipe);
    SM64_PTR(const BehaviorScript*, bhvWhitePuffExplosion);
    SM64_PTR(const BehaviorScript*, bhvSpawnedStar);
    SM64_PTR(const BehaviorScript*, bhvSpawnedStarNoLevelExit);
    SM64_PTR(const BehaviorScript*, bhvSpawnedBlueCoin);
    SM64_PTR(const BehaviorScript*, bhvCoinInsideBoo);
    SM64_PTR(const BehaviorScript*, bhvCoinFormationSpawn);
    SM64_PTR(const BehaviorScript*, bhvCoinFormation);
    SM64_PTR(const BehaviorScript*, bhvOneCoin);
    SM64_PTR(const BehaviorScript*, bhvYellowCoin);
    SM64_PTR(const BehaviorScript*, bhvTemporaryYellowCoin);
    SM64_PTR(const BehaviorScript*, bhvThreeCoinsSpawn);
    SM64_PTR(const BehaviorScript*, bhvTenCoinsSpawn);
    SM64_PTR(const BehaviorScript*, bhvSingleCoinGetsSpawned);
    SM64_PTR(const BehaviorScript*, bhvCoinSparkles);
    SM64_PTR(const BehaviorScript*, bhvGoldenCoinSparkles);
    SM64_PTR(const BehaviorScript*, bhvWallTinyStarParticle);
    SM64_PTR(const BehaviorScript*, bhvVertStarParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvPoundTinyStarParticle);
    SM64_PTR(const BehaviorScript*, bhvHorStarParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvPunchTinyTriangle);
    SM64_PTR(const BehaviorScript*, bhvTriangleParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvDoorWarp);
    SM64_PTR(const BehaviorScript*, bhvDoor);
    SM64_PTR(const BehaviorScript*, bhvGrindel);
    SM64_PTR(const BehaviorScript*, bhvThwomp);
    SM64_PTR(const BehaviorScript*, bhvThwomp2);
    SM64_PTR(const BehaviorScript*, bhvTumblingBridgePlatform);
    SM64_PTR(const BehaviorScript*, bhvTumblingBridge);
    SM64_PTR(const BehaviorScript*, bhvBBHTumblingBridge);
    SM64_PTR(const BehaviorScript*, bhvLLLTumblingBridge);
    SM64_PTR(const BehaviorScript*, bhvFlame);
    SM64_PTR(const BehaviorScript*, bhvAnotherElavator);
    SM64_PTR(const BehaviorScript*, bhvRRElevatorPlatform);
    SM64_PTR(const BehaviorScript*, bhvHMCElevatorPlatform);
    SM64_PTR(const BehaviorScript*, bhvWaterMist);
    SM64_PTR(const BehaviorScript*, bhvBreathParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvBreakBoxTriangle);
    SM64_PTR(const BehaviorScript*, bhvWaterMist2);
    SM64_PTR(const BehaviorScript*, bhvUnused0DFC);
    SM64_PTR(const BehaviorScript*, bhvMistCircParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvDirtParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvUnused0E40);
    SM64_PTR(const BehaviorScript*, bhvSnowParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvWind);
    SM64_PTR(const BehaviorScript*, bhvEndToad);
    SM64_PTR(const BehaviorScript*, bhvEndPeach);
    SM64_PTR(const BehaviorScript*, bhvUnusedParticleSpawn);
    SM64_PTR(const BehaviorScript*, bhvUkiki);
    SM64_PTR(const BehaviorScript*, bhvUkikiCageChild);
    SM64_PTR(const BehaviorScript*, bhvUkikiCageStar);
    SM64_PTR(const BehaviorScript*, bhvUkikiCage);
    SM64_PTR(const BehaviorScript*, bhvBitFSSinkingPlatforms);
    SM64_PTR(const BehaviorScript*, bhvBitFSSinkingCagePlatform);
    SM64_PTR(const BehaviorScript*, bhvDDDMovingPole);
    SM64_PTR(const BehaviorScript*, bhvBitFSTiltingInvertedPyramid);
    SM64_PTR(const BehaviorScript*, bhvSquishablePlatform);
    SM64_PTR(const BehaviorScript*, bhvCutOutObject);
    SM64_PTR(const BehaviorScript*, bhvBetaMovingFlamesSpawn);
    SM64_PTR(const BehaviorScript*, bhvBetaMovingFlames);
    SM64_PTR(const BehaviorScript*, bhvRRRotatingBridgePlatform);
    SM64_PTR(const BehaviorScript*, bhvFlamethrower);
    SM64_PTR(const BehaviorScript*, bhvFlamethrowerFlame);
    SM64_PTR(const BehaviorScript*, bhvBouncingFireball);
    SM64_PTR(const BehaviorScript*, bhvBouncingFireballFlame);
    SM64_PTR(const BehaviorScript*, bhvBowserShockWave);
    SM64_PTR(const BehaviorScript*, bhvFireParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvBlackSmokeMario);
    SM64_PTR(const BehaviorScript*, bhvBlackSmokeBowser);
    SM64_PTR(const BehaviorScript*, bhvBlackSmokeUpward);
    SM64_PTR(const BehaviorScript*, bhvBetaFishSplashSpawner);
    SM64_PTR(const BehaviorScript*, bhvSpindrift);
    SM64_PTR(const BehaviorScript*, bhvTowerPlatformGroup);
    SM64_PTR(const BehaviorScript*, bhvWFSlidingTowerPlatform);
    SM64_PTR(const BehaviorScript*, bhvWFElevatorTowerPlatform);
    SM64_PTR(const BehaviorScript*, bhvWFSolidTowerPlatform);
    SM64_PTR(const BehaviorScript*, bhvLeafParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvTreeSnow);
    SM64_PTR(const BehaviorScript*, bhvTreeLeaf);
    SM64_PTR(const BehaviorScript*, bhvAnotherTiltingPlatform);
    SM64_PTR(const BehaviorScript*, bhvSquarishPathMoving);
    SM64_PTR(const BehaviorScript*, bhvPiranhaPlantBubble);
    SM64_PTR(const BehaviorScript*, bhvPiranhaPlantWakingBubbles);
    SM64_PTR(const BehaviorScript*, bhvFloorSwitchAnimatesObject);
    SM64_PTR(const BehaviorScript*, bhvFloorSwitchGrills);
    SM64_PTR(const BehaviorScript*, bhvFloorSwitchHardcodedModel);
    SM64_PTR(const BehaviorScript*, bhvFloorSwitchHiddenObjects);
    SM64_PTR(const BehaviorScript*, bhvHiddenObject);
    SM64_PTR(const BehaviorScript*, bhvBreakableBox);
    SM64_PTR(const BehaviorScript*, bhvPushableMetalBox);
    SM64_PTR(const BehaviorScript*, bhvHeaveHo);
    SM64_PTR(const BehaviorScript*, bhvHeaveHoThrowMario);
    SM64_PTR(const BehaviorScript*, bhvCCMTouchedStarSpawn);
    SM64_PTR(const BehaviorScript*, bhvUnusedPoundablePlatform);
    SM64_PTR(const BehaviorScript*, bhvBetaTrampolineTop);
    SM64_PTR(const BehaviorScript*, bhvBetaTrampolineSpring);
    SM64_PTR(const BehaviorScript*, bhvJumpingBox);
    SM64_PTR(const BehaviorScript*, bhvBooCage);
    SM64_PTR(const BehaviorScript*, bhvStub);
    SM64_PTR(const BehaviorScript*, bhvIgloo);
    SM64_PTR(const BehaviorScript*, bhvBowserKey);
    SM64_PTR(const BehaviorScript*, bhvGrandStar);
    SM64_PTR(const BehaviorScript*, bhvBetaBooKey);
    SM64_PTR(const BehaviorScript*, bhvAlphaBooKey);
    SM64_PTR(const BehaviorScript*, bhvBulletBill);
    SM64_PTR(const BehaviorScript*, bhvWhitePuffSmoke);
    SM64_PTR(const BehaviorScript*, bhvUnused1820);
    SM64_PTR(const BehaviorScript*, bhvBowserTailAnchor);
    SM64_PTR(const BehaviorScript*, bhvBowser);
    SM64_PTR(const BehaviorScript*, bhvBowserBodyAnchor);
    SM64_PTR(const BehaviorScript*, bhvBowserFlameSpawn);
    SM64_PTR(const BehaviorScript*, bhvTiltingBowserLavaPlatform);
    SM64_PTR(const BehaviorScript*, bhvFallingBowserPlatform);
    SM64_PTR(const BehaviorScript*, bhvBlueBowserFlame);
    SM64_PTR(const BehaviorScript*, bhvFlameFloatingLanding);
    SM64_PTR(const BehaviorScript*, bhvBlueFlamesGroup);
    SM64_PTR(const BehaviorScript*, bhvFlameBouncing);
    SM64_PTR(const BehaviorScript*, bhvFlameMovingForwardGrowing);
    SM64_PTR(const BehaviorScript*, bhvFlameBowser);
    SM64_PTR(const BehaviorScript*, bhvFlameLargeBurningOut);
    SM64_PTR(const BehaviorScript*, bhvBlueFish);
    SM64_PTR(const BehaviorScript*, bhvTankFishGroup);
    SM64_PTR(const BehaviorScript*, bhvCheckerboardElevatorGroup);
    SM64_PTR(const BehaviorScript*, bhvCheckerboardPlatformSub);
    SM64_PTR(const BehaviorScript*, bhvBowserKeyUnlockDoor);
    SM64_PTR(const BehaviorScript*, bhvBowserKeyCourseExit);
    SM64_PTR(const BehaviorScript*, bhvInvisibleObjectsUnderBridge);
    SM64_PTR(const BehaviorScript*, bhvWaterLevelPillar);
    SM64_PTR(const BehaviorScript*, bhvDDDWarp);
    SM64_PTR(const BehaviorScript*, bhvMoatGrills);
    SM64_PTR(const BehaviorScript*, bhvClockMinuteHand);
    SM64_PTR(const BehaviorScript*, bhvClockHourHand);
    SM64_PTR(const BehaviorScript*, bhvMacroUkiki);
    SM64_PTR(const BehaviorScript*, bhvStub1D0C);
    SM64_PTR(const BehaviorScript*, bhvLLLRotatingHexagonalPlatform);
    SM64_PTR(const BehaviorScript*, bhvLLLSinkingRockBlock);
    SM64_PTR(const BehaviorScript*, bhvStub1D70);
    SM64_PTR(const BehaviorScript*, bhvLLLMovingOctagonalMeshPlatform);
    SM64_PTR(const BehaviorScript*, bhvSnowBall);
    SM64_PTR(const BehaviorScript*, bhvLLLRotatingBlockWithFireBars);
    SM64_PTR(const BehaviorScript*, bhvLLLRotatingHexFlame);
    SM64_PTR(const BehaviorScript*, bhvLLLWoodPiece);
    SM64_PTR(const BehaviorScript*, bhvLLLFloatingWoodBridge);
    SM64_PTR(const BehaviorScript*, bhvVolcanoFlames);
    SM64_PTR(const BehaviorScript*, bhvLLLRotatingHexagonalRing);
    SM64_PTR(const BehaviorScript*, bhvLLLSinkingRectangularPlatform);
    SM64_PTR(const BehaviorScript*, bhvLLLSinkingSquarePlatforms);
    SM64_PTR(const BehaviorScript*, bhvLLLTiltingInvertedPyramid);
    SM64_PTR(const BehaviorScript*, bhvUnused1F30);
    SM64_PTR(const BehaviorScript*, bhvKoopaShell);
    SM64_PTR(const BehaviorScript*, bhvKoopaShellFlame);
    SM64_PTR(const BehaviorScript*, bhvToxBox);
    SM64_PTR(const BehaviorScript*, bhvPiranhaPlant);
    SM64_PTR(const BehaviorScript*, bhvLLLHexagonalMesh);
    SM64_PTR(const BehaviorScript*, bhvLLLBowserPuzzlePiece);
    SM64_PTR(const BehaviorScript*, bhvLLLBowserPuzzle);
    SM64_PTR(const BehaviorScript*, bhvTuxiesMother);
    SM64_PTR(const BehaviorScript*, bhvPenguinBaby);
    SM64_PTR(const BehaviorScript*, bhvUnused20E0);
    SM64_PTR(const BehaviorScript*, bhvSmallPenguin);
    SM64_PTR(const BehaviorScript*, bhvManyBlueFishSpawner);
    SM64_PTR(const BehaviorScript*, bhvFewBlueFishSpawner);
    SM64_PTR(const BehaviorScript*, bhvFishSpawner);
    SM64_PTR(const BehaviorScript*, bhvFishCommon);
    SM64_PTR(const BehaviorScript*, bhvFish);
    SM64_PTR(const BehaviorScript*, bhvWDWExpressElevator);
    SM64_PTR(const BehaviorScript*, bhvWDWExpressElevatorPlatform);
    SM64_PTR(const BehaviorScript*, bhvChirpChirp);
    SM64_PTR(const BehaviorScript*, bhvChirpChirpUnused);
    SM64_PTR(const BehaviorScript*, bhvBub);
    SM64_PTR(const BehaviorScript*, bhvCheepCheep);
    SM64_PTR(const BehaviorScript*, bhvExclamationBox);
    SM64_PTR(const BehaviorScript*, bhvRotatingExclamationMark);
    SM64_PTR(const BehaviorScript*, bhvPlaysSound);
    SM64_PTR(const BehaviorScript*, bhvSoundSpawner);
    SM64_PTR(const BehaviorScript*, bhvRockSolid);
    SM64_PTR(const BehaviorScript*, bhvBowserSubDoor);
    SM64_PTR(const BehaviorScript*, bhvBowsersSub);
    SM64_PTR(const BehaviorScript*, bhvSushiShark);
    SM64_PTR(const BehaviorScript*, bhvSushiSharkCollisionChild);
    SM64_PTR(const BehaviorScript*, bhvJRBSlidingBox);
    SM64_PTR(const BehaviorScript*, bhvShipPart3);
    SM64_PTR(const BehaviorScript*, bhvInSunkenShip3);
    SM64_PTR(const BehaviorScript*, bhvSunkenShipPart);
    SM64_PTR(const BehaviorScript*, bhvUnused243C);
    SM64_PTR(const BehaviorScript*, bhvSunkenShipPart2);
    SM64_PTR(const BehaviorScript*, bhvInSunkenShip);
    SM64_PTR(const BehaviorScript*, bhvInSunkenShip2);
    SM64_PTR(const BehaviorScript*, bhvMistParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvWhitePuff1);
    SM64_PTR(const BehaviorScript*, bhvWhitePuff2);
    SM64_PTR(const BehaviorScript*, bhvWhitePuffSmoke2);
    SM64_PTR(const BehaviorScript*, bhvPurpleSwitchHiddenBoxes);
    SM64_PTR(const BehaviorScript*, bhvBlueCoinSwitch);
    SM64_PTR(const BehaviorScript*, bhvHiddenBlueCoin);
    SM64_PTR(const BehaviorScript*, bhvOpenableCageDoor);
    SM64_PTR(const BehaviorScript*, bhvOpenableGrill);
    SM64_PTR(const BehaviorScript*, bhvWaterLevelDiamond);
    SM64_PTR(const BehaviorScript*, bhvInitializeChangingWaterLevel);
    SM64_PTR(const BehaviorScript*, bhvTweesterSandParticle);
    SM64_PTR(const BehaviorScript*, bhvTweester);
    SM64_PTR(const BehaviorScript*, bhvMerryGoRoundBooManager);
    SM64_PTR(const BehaviorScript*, bhvPlaysMusicTrackWhenTouched);
    SM64_PTR(const BehaviorScript*, bhvAnimatedTexture);
    SM64_PTR(const BehaviorScript*, bhvBooInCastle);
    SM64_PTR(const BehaviorScript*, bhvBooWithCage);
    SM64_PTR(const BehaviorScript*, bhvBalconyBigBoo);
    SM64_PTR(const BehaviorScript*, bhvMerryGoRoundBigBoo);
    SM64_PTR(const BehaviorScript*, bhvGhostHuntBigBoo);
    SM64_PTR(const BehaviorScript*, bhvCourtyardBooTriplet);
    SM64_PTR(const BehaviorScript*, bhvBoo);
    SM64_PTR(const BehaviorScript*, bhvMerryGoRoundBoo);
    SM64_PTR(const BehaviorScript*, bhvGhostHuntBoo);
    SM64_PTR(const BehaviorScript*, bhvHiddenStaircaseStep);
    SM64_PTR(const BehaviorScript*, bhvBooStaircase);
    SM64_PTR(const BehaviorScript*, bhvBBHTiltingTrapPlatform);
    SM64_PTR(const BehaviorScript*, bhvHauntedBookshelf);
    SM64_PTR(const BehaviorScript*, bhvMeshElevator);
    SM64_PTR(const BehaviorScript*, bhvMerryGoRound);
    SM64_PTR(const BehaviorScript*, bhvInsideCannon);
    SM64_PTR(const BehaviorScript*, bhvBetaBowserAnchor);
    SM64_PTR(const BehaviorScript*, bhvStaticCheckeredPlatform);
    SM64_PTR(const BehaviorScript*, bhvUnused2A10);
    SM64_PTR(const BehaviorScript*, bhvStar);
    SM64_PTR(const BehaviorScript*, bhvStaticObject);
    SM64_PTR(const BehaviorScript*, bhvUnused2A54);
    SM64_PTR(const BehaviorScript*, bhvCastleFloorTrap);
    SM64_PTR(const BehaviorScript*, bhvFloorTrapInCastle);
    SM64_PTR(const BehaviorScript*, bhvTree);
    SM64_PTR(const BehaviorScript*, bhvSparkle);
    SM64_PTR(const BehaviorScript*, bhvSparkleSpawn);
    SM64_PTR(const BehaviorScript*, bhvSparkleParticleSpawner);
    SM64_PTR(const BehaviorScript*, bhvSomeGfx);
    SM64_PTR(const BehaviorScript*, bhvScuttlebug);
    SM64_PTR(const BehaviorScript*, bhvScuttlebugSpawn);
    SM64_PTR(const BehaviorScript*, bhvWhompKingBoss);
    SM64_PTR(const BehaviorScript*, bhvSmallWhomp);
    SM64_PTR(const BehaviorScript*, bhvWaterSplash);
    SM64_PTR(const BehaviorScript*, bhvWaterDroplet);
    SM64_PTR(const BehaviorScript*, bhvWaterDropletSplash);
    SM64_PTR(const BehaviorScript*, bhvBubbleSplash);
    SM64_PTR(const BehaviorScript*, bhvIdleWaterWave);
    SM64_PTR(const BehaviorScript*, bhvObjectWaterSplash);
    SM64_PTR(const BehaviorScript*, bhvShallowWaterWave);
    SM64_PTR(const BehaviorScript*, bhvShallowWaterSplash);
    SM64_PTR(const BehaviorScript*, bhvObjectWaveTrail);
    SM64_PTR(const BehaviorScript*, bhvWaveTrail);
    SM64_PTR(const BehaviorScript*, bhvTinyStrongWindParticle);
    SM64_PTR(const BehaviorScript*, bhvStrongWindParticle);
    SM64_PTR(const BehaviorScript*, bhvSLSnowmanWind);
    SM64_PTR(const BehaviorScript*, bhvSLWalkingPenguin);
    SM64_PTR(const BehaviorScript*, bhvYellowBall);
    SM64_PTR(const BehaviorScript*, bhvMario);
    SM64_PTR(const BehaviorScript*, bhvToadMessage);
    SM64_PTR(const BehaviorScript*, bhvUnlockDoorStar);
    SM64_PTR(const BehaviorScript*, bhvInstantActiveWarp);
    SM64_PTR(const BehaviorScript*, bhvAirborneWarp);
    SM64_PTR(const BehaviorScript*, bhvHardAirKnockBackWarp);
    SM64_PTR(const BehaviorScript*, bhvSpinAirborneCircleWarp);
    SM64_PTR(const BehaviorScript*, bhvDeathWarp);
    SM64_PTR(const BehaviorScript*, bhvSpinAirborneWarp);
    SM64_PTR(const BehaviorScript*, bhvFlyingWarp);
    SM64_PTR(const BehaviorScript*, bhvPaintingStarCollectWarp);
    SM64_PTR(const BehaviorScript*, bhvPaintingDeathWarp);
    SM64_PTR(const BehaviorScript*, bhvAirborneDeathWarp);
    SM64_PTR(const BehaviorScript*, bhvAirborneStarCollectWarp);
    SM64_PTR(const BehaviorScript*, bhvLaunchStarCollectWarp);
    SM64_PTR(const BehaviorScript*, bhvLaunchDeathWarp);
    SM64_PTR(const BehaviorScript*, bhvSwimmingWarp);
    SM64_PTR(const BehaviorScript*, bhvRandomAnimatedTexture);
    SM64_PTR(const BehaviorScript*, bhvYellowBackgroundInMenu);
    SM64_PTR(const BehaviorScript*, bhvMenuButton);
    SM64_PTR(const BehaviorScript*, bhvMenuButtonManager);
    SM64_PTR(const BehaviorScript*, bhvActSelectorStarType);
    SM64_PTR(const BehaviorScript*, bhvActSelector);
    SM64_PTR(const BehaviorScript*, bhvMovingYellowCoin);
    SM64_PTR(const BehaviorScript*, bhvMovingBlueCoin);
    SM64_PTR(const BehaviorScript*, bhvBlueCoinSliding);
    SM64_PTR(const BehaviorScript*, bhvBlueCoinJumping);
    SM64_PTR(const BehaviorScript*, bhvSeaweed);
    SM64_PTR(const BehaviorScript*, bhvSeaweedBundle);
    SM64_PTR(const BehaviorScript*, bhvBobomb);
    SM64_PTR(const BehaviorScript*, bhvBobombFuseSmoke);
    SM64_PTR(const BehaviorScript*, bhvBobombBuddy);
    SM64_PTR(const BehaviorScript*, bhvBobombBuddyOpensCannon);
    SM64_PTR(const BehaviorScript*, bhvCannonClosed);
    SM64_PTR(const BehaviorScript*, bhvWhirlpool);
    SM64_PTR(const BehaviorScript*, bhvJetStream);
    SM64_PTR(const BehaviorScript*, bhvMessagePanel);
    SM64_PTR(const BehaviorScript*, bhvSignOnWall);
    SM64_PTR(const BehaviorScript*, bhvHomingAmp);
    SM64_PTR(const BehaviorScript*, bhvCirclingAmp);
    SM64_PTR(const BehaviorScript*, bhvButterfly);
    SM64_PTR(const BehaviorScript*, bhvHoot);
    SM64_PTR(const BehaviorScript*, bhvBetaHoldableObject);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething1);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething2);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething3);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething4);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething5);
    SM64_PTR(const BehaviorScript*, bhvCarrySomething6);
    SM64_PTR(const BehaviorScript*, bhvObjectBubble);
    SM64_PTR(const BehaviorScript*, bhvObjectWaterWave);
    SM64_PTR(const BehaviorScript*, bhvExplosion);
    SM64_PTR(const BehaviorScript*, bhvBobombBullyDeathSmoke);
    SM64_PTR(const BehaviorScript*, bhvSmoke);
    SM64_PTR(const BehaviorScript*, bhvBobombExplosionBubble);
    SM64_PTR(const BehaviorScript*, bhvBobombExplosionBubble3600);
    SM64_PTR(const BehaviorScript*, bhvRespawner);
    SM64_PTR(const BehaviorScript*, bhvSmallBully);
    SM64_PTR(const BehaviorScript*, bhvBigBully);
    SM64_PTR(const BehaviorScript*, bhvBigBullyWithMinions);
    SM64_PTR(const BehaviorScript*, bhvSmallChillBully);
    SM64_PTR(const BehaviorScript*, bhvBigChillBully);
    SM64_PTR(const BehaviorScript*, bhvJetStreamRingSpawner);
    SM64_PTR(const BehaviorScript*, bhvJetStreamWaterRing);
    SM64_PTR(const BehaviorScript*, bhvMantaRayWaterRing);
    SM64_PTR(const BehaviorScript*, bhvMantaRayRingManager);
    SM64_PTR(const BehaviorScript*, bhvBowserBomb);
    SM64_PTR(const BehaviorScript*, bhvBowserBombExplosion);
    SM64_PTR(const BehaviorScript*, bhvBowserBombSmoke);
    SM64_PTR(const BehaviorScript*, bhvCelebrationStar);
    SM64_PTR(const BehaviorScript*, bhvCelebrationStarSparkle);
    SM64_PTR(const BehaviorScript*, bhvStarKeyCollectionPuffSpawner);
    SM64_PTR(const BehaviorScript*, bhvLLLDrawbridgeSpawner);
    SM64_PTR(const BehaviorScript*, bhvLLLDrawbridge);
    SM64_PTR(const BehaviorScript*, bhvSmallBomp);
    SM64_PTR(const BehaviorScript*, bhvLargeBomp);
    SM64_PTR(const BehaviorScript*, bhvWFSlidingPlatform);
    SM64_PTR(const BehaviorScript*, bhvMoneybag);
    SM64_PTR(const BehaviorScript*, bhvMoneybagHidden);
    SM64_PTR(const BehaviorScript*, bhvPitBowlingBall);
    SM64_PTR(const BehaviorScript*, bhvFreeBowlingBall);
    SM64_PTR(const BehaviorScript*, bhvBowlingBall);
    SM64_PTR(const BehaviorScript*, bhvTTMBowlingBallSpawner);
    SM64_PTR(const BehaviorScript*, bhvBoBBowlingBallSpawner);
    SM64_PTR(const BehaviorScript*, bhvTHIBowlingBallSpawner);
    SM64_PTR(const BehaviorScript*, bhvRRCruiserWing);
    SM64_PTR(const BehaviorScript*, bhvSpindel);
    SM64_PTR(const BehaviorScript*, bhvSSLMovingPyramidWall);
    SM64_PTR(const BehaviorScript*, bhvPyramidElevator);
    SM64_PTR(const BehaviorScript*, bhvPyramidElevatorTrajectoryMarkerBall);
    SM64_PTR(const BehaviorScript*, bhvPyramidTop);
    SM64_PTR(const BehaviorScript*, bhvPyramidTopFragment);
    SM64_PTR(const BehaviorScript*, bhvPyramidPillarTouchDetector);
    SM64_PTR(const BehaviorScript*, bhvWaterfallSoundLoop);
    SM64_PTR(const BehaviorScript*, bhvVolcanoSoundLoop);
    SM64_PTR(const BehaviorScript*, bhvCastleFlagWaving);
    SM64_PTR(const BehaviorScript*, bhvBirdsSoundLoop);
    SM64_PTR(const BehaviorScript*, bhvAmbientSounds);
    SM64_PTR(const BehaviorScript*, bhvSandSoundLoop);
    SM64_PTR(const BehaviorScript*, bhvHiddenAt120Stars);
    SM64_PTR(const BehaviorScript*, bhvSnowmansBottom);
    SM64_PTR(const BehaviorScript*, bhvSnowmansHead);
    SM64_PTR(const BehaviorScript*, bhvSnowmansBodyCheckpoint);
    SM64_PTR(const BehaviorScript*, bhvBigSnowmanWhole);
    SM64_PTR(const BehaviorScript*, bhvBigBoulder);
    SM64_PTR(const BehaviorScript*, bhvBigBoulderGenerator);
    SM64_PTR(const BehaviorScript*, bhvWingCap);
    SM64_PTR(const BehaviorScript*, bhvMetalCap);
    SM64_PTR(const BehaviorScript*, bhvNormalCap);
    SM64_PTR(const BehaviorScript*, bhvVanishCap);
    SM64_PTR(const BehaviorScript*, bhvStarSpawnCoordinates);
    SM64_PTR(const BehaviorScript*, bhvHiddenRedCoinStar);
    SM64_PTR(const BehaviorScript*, bhvRedCoin);
    SM64_PTR(const BehaviorScript*, bhvBowserCourseRedCoinStar);
    SM64_PTR(const BehaviorScript*, bhvHiddenStar);
    SM64_PTR(const BehaviorScript*, bhvHiddenStarTrigger);
    SM64_PTR(const BehaviorScript*, bhvTTMRollingLog);
    SM64_PTR(const BehaviorScript*, bhvLLLVolcanoFallingTrap);
    SM64_PTR(const BehaviorScript*, bhvLLLRollingLog);
    SM64_PTR(const BehaviorScript*, bhv1UpWalking);
    SM64_PTR(const BehaviorScript*, bhv1UpRunningAway);
    SM64_PTR(const BehaviorScript*, bhv1UpSliding);
    SM64_PTR(const BehaviorScript*, bhv1Up);
    SM64_PTR(const BehaviorScript*, bhv1UpJumpOnApproach);
    SM64_PTR(const BehaviorScript*, bhvHidden1Up);
    SM64_PTR(const BehaviorScript*, bhvHidden1UpTrigger);
    SM64_PTR(const BehaviorScript*, bhvHidden1UpInPole);
    SM64_PTR(const BehaviorScript*, bhvHidden1UpInPoleTrigger);
    SM64_PTR(const BehaviorScript*, bhvHidden1UpInPoleSpawner);
    SM64_PTR(const BehaviorScript*, bhvControllablePlatform);
    SM64_PTR(const BehaviorScript*, bhvControllablePlatformSub);
    SM64_PTR(const BehaviorScript*, bhvBreakableBoxSmall);
    SM64_PTR(const BehaviorScript*, bhvSlidingSnowMound);
    SM64_PTR(const BehaviorScript*, bhvSnowMoundSpawn);
    SM64_PTR(const BehaviorScript*, bhvWDWSquareFloatingPlatform);
    SM64_PTR(const BehaviorScript*, bhvWDWRectangularFloatingPlatform);
    SM64_PTR(const BehaviorScript*, bhvJRBFloatingPlatform);
    SM64_PTR(const BehaviorScript*, bhvArrowLift);
    SM64_PTR(const BehaviorScript*, bhvOrangeNumber);
    SM64_PTR(const BehaviorScript*, bhvMantaRay);
    SM64_PTR(const BehaviorScript*, bhvFallingPillar);
    SM64_PTR(const BehaviorScript*, bhvFallingPillarHitbox);
    SM64_PTR(const BehaviorScript*, bhvPillarBase);
    SM64_PTR(const BehaviorScript*, bhvJRBFloatingBox);
    SM64_PTR(const BehaviorScript*, bhvDecorativePendulum);
    SM64_PTR(const BehaviorScript*, bhvTreasureChestsShip);
    SM64_PTR(const BehaviorScript*, bhvTreasureChestsJRB);
    SM64_PTR(const BehaviorScript*, bhvTreasureChestsDDD);
    SM64_PTR(const BehaviorScript*, bhvTreasureChestBottom);
    SM64_PTR(const BehaviorScript*, bhvTreasureChestTop);
    SM64_PTR(const BehaviorScript*, bhvMips);
    SM64_PTR(const BehaviorScript*, bhvYoshi);
    SM64_PTR(const BehaviorScript*, bhvKoopa);
    SM64_PTR(const BehaviorScript*, bhvKoopaRaceEndpoint);
    SM64_PTR(const BehaviorScript*, bhvKoopaFlag);
    SM64_PTR(const BehaviorScript*, bhvPokey);
    SM64_PTR(const BehaviorScript*, bhvPokeyBodyPart);
    SM64_PTR(const BehaviorScript*, bhvSwoop);
    SM64_PTR(const BehaviorScript*, bhvFlyGuy);
    SM64_PTR(const BehaviorScript*, bhvGoomba);
    SM64_PTR(const BehaviorScript*, bhvGoombaTripletSpawner);
    SM64_PTR(const BehaviorScript*, bhvChainChomp);
    SM64_PTR(const BehaviorScript*, bhvChainChompChainPart);
    SM64_PTR(const BehaviorScript*, bhvWoodenPost);
    SM64_PTR(const BehaviorScript*, bhvChainChompGate);
    SM64_PTR(const BehaviorScript*, bhvWigglerHead);
    SM64_PTR(const BehaviorScript*, bhvWigglerBody);
    SM64_PTR(const BehaviorScript*, bhvEnemyLakitu);
    SM64_PTR(const BehaviorScript*, bhvCameraLakitu);
    SM64_PTR(const BehaviorScript*, bhvCloud);
    SM64_PTR(const BehaviorScript*, bhvCloudPart);
    SM64_PTR(const BehaviorScript*, bhvSpiny);
    SM64_PTR(const BehaviorScript*, bhvMontyMole);
    SM64_PTR(const BehaviorScript*, bhvMontyMoleHole);
    SM64_PTR(const BehaviorScript*, bhvMontyMoleRock);
    SM64_PTR(const BehaviorScript*, bhvPlatformOnTrack);
    SM64_PTR(const BehaviorScript*, bhvTrackBall);
    SM64_PTR(const BehaviorScript*, bhvSeesawPlatform);
    SM64_PTR(const BehaviorScript*, bhvFerrisWheelAxle);
    SM64_PTR(const BehaviorScript*, bhvFerrisWheelPlatform);
    SM64_PTR(const BehaviorScript*, bhvWaterBombSpawner);
    SM64_PTR(const BehaviorScript*, bhvWaterBomb);
    SM64_PTR(const BehaviorScript*, bhvWaterBombShadow);
    SM64_PTR(const BehaviorScript*, bhvTTCRotatingSolid);
    SM64_PTR(const BehaviorScript*, bhvTTCPendulum);
    SM64_PTR(const BehaviorScript*, bhvTTCTreadmill);
    SM64_PTR(const BehaviorScript*, bhvTTCMovingBar);
    SM64_PTR(const BehaviorScript*, bhvTTCCog);
    SM64_PTR(const BehaviorScript*, bhvTTCPitBlock);
    SM64_PTR(const BehaviorScript*, bhvTTCElevator);
    SM64_PTR(const BehaviorScript*, bhvTTC2DRotator);
    SM64_PTR(const BehaviorScript*, bhvTTCSpinner);
    SM64_PTR(const BehaviorScript*, bhvMrBlizzard);
    SM64_PTR(const BehaviorScript*, bhvMrBlizzardSnowball);
    SM64_PTR(const BehaviorScript*, bhvSlidingPlatform2);
    SM64_PTR(const BehaviorScript*, bhvOctagonalPlatformRotating);
    SM64_PTR(const BehaviorScript*, bhvAnimatesOnFloorSwitchPress);
    SM64_PTR(const BehaviorScript*, bhvActivatedBackAndForthPlatform);
    SM64_PTR(const BehaviorScript*, bhvRecoveryHeart);
    SM64_PTR(const BehaviorScript*, bhvWaterBombCannon);
    SM64_PTR(const BehaviorScript*, bhvCannonBarrelBubbles);
    SM64_PTR(const BehaviorScript*, bhvUnagi);
    SM64_PTR(const BehaviorScript*, bhvUnagiSubobject);
    SM64_PTR(const BehaviorScript*, bhvDorrie);
    SM64_PTR(const BehaviorScript*, bhvHauntedChair);
    SM64_PTR(const BehaviorScript*, bhvMadPiano);
    SM64_PTR(const BehaviorScript*, bhvFlyingBookend);
    SM64_PTR(const BehaviorScript*, bhvBookendSpawn);
    SM64_PTR(const BehaviorScript*, bhvHauntedBookshelfManager);
    SM64_PTR(const BehaviorScript*, bhvBookSwitch);
    SM64_PTR(const BehaviorScript*, bhvFirePiranhaPlant);
    SM64_PTR(const BehaviorScript*, bhvSmallPiranhaFlame);
    SM64_PTR(const BehaviorScript*, bhvFireSpitter);
    SM64_PTR(const BehaviorScript*, bhvFlyguyFlame);
    SM64_PTR(const BehaviorScript*, bhvSnufit);
    SM64_PTR(const BehaviorScript*, bhvSnufitBalls);
    SM64_PTR(const BehaviorScript*, bhvHorizontalGrindel);
    SM64_PTR(const BehaviorScript*, bhvEyerokBoss);
    SM64_PTR(const BehaviorScript*, bhvEyerokHand);
    SM64_PTR(const BehaviorScript*, bhvKlepto);
    SM64_PTR(const BehaviorScript*, bhvBird);
    SM64_PTR(const BehaviorScript*, bhvRacingPenguin);
    SM64_PTR(const BehaviorScript*, bhvPenguinRaceFinishLine);
    SM64_PTR(const BehaviorScript*, bhvPenguinRaceShortcutCheck);
    SM64_PTR(const BehaviorScript*, bhvCoffinSpawner);
    SM64_PTR(const BehaviorScript*, bhvCoffin);
    SM64_PTR(const BehaviorScript*, bhvClamShell);
    SM64_PTR(const BehaviorScript*, bhvSkeeter);
    SM64_PTR(const BehaviorScript*, bhvSkeeterWave);
    SM64_PTR(const BehaviorScript*, bhvSwingPlatform);
    SM64_PTR(const BehaviorScript*, bhvDonutPlatformSpawner);
    SM64_PTR(const BehaviorScript*, bhvDonutPlatform);
    SM64_PTR(const BehaviorScript*, bhvDDDPole);
    SM64_PTR(const BehaviorScript*, bhvRedCoinStarMarker);
    SM64_PTR(const BehaviorScript*, bhvTripletButterfly);
    SM64_PTR(const BehaviorScript*, bhvBubba);
    SM64_PTR(const BehaviorScript*, bhvBeginningLakitu);
    SM64_PTR(const BehaviorScript*, bhvBeginningPeach);
    SM64_PTR(const BehaviorScript*, bhvEndBirds1);
    SM64_PTR(const BehaviorScript*, bhvEndBirds2);
    SM64_PTR(const BehaviorScript*, bhvIntroScene);
    SM64_PTR(const BehaviorScript*, bhvUnusedFakeStar);
#pragma endregion

#undef SM64_GLOBAL
#undef SM64_PTR
  };
}  // namespace sm64

#endif