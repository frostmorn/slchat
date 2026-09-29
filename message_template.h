#pragma once

#include <stdint.h>

#define MK_HIGH(n)  (0x00000u | (n))
#define MK_MED(n)   (0x10000u | (n))
#define MK_LOW(n)   (0x20000u | (n))
#define MK_FIXED(n) (0x30000u | (n))

enum {

    MSG_TestMessage                      = MK_LOW(1), // Test1: U32, Test0: U32, Test1: U32, Test2: U32
    MSG_PacketAck                        = MK_FIXED(0xFFFFFFFB), // ID: U32
    MSG_OpenCircuit                      = MK_FIXED(0xFFFFFFFC), // IP: IPADDR, Port: IPPORT
    MSG_CloseCircuit                     = MK_FIXED(0xFFFFFFFD),
    MSG_StartPingCheck                   = MK_HIGH(1), // PingID: U8, OldestUnacked: U32
    MSG_CompletePingCheck                = MK_HIGH(2), // PingID: U8
    MSG_AddCircuitCode                   = MK_LOW(2), // Code: U32, SessionID: LLUUID, AgentID: LLUUID
    MSG_UseCircuitCode                   = MK_LOW(3), // Code: U32, SessionID: LLUUID, ID: LLUUID
    MSG_NeighborList                     = MK_HIGH(3), // IP: IPADDR, Port: IPPORT, PublicIP: IPADDR, PublicPort: IPPORT, RegionID: LLUUID, SimAccess: U8
    MSG_AvatarTextureUpdate              = MK_LOW(4), // AgentID: LLUUID, TexturesChanged: BOOL, CacheID: LLUUID, TextureIndex: U8, TextureID: LLUUID
    MSG_SimulatorMapUpdate               = MK_LOW(5), // Flags: U32
    MSG_SimulatorSetMap                  = MK_LOW(6), // RegionHandle: U64, Type: S32, MapImage: LLUUID
    MSG_SubscribeLoad                    = MK_LOW(7),
    MSG_UnsubscribeLoad                  = MK_LOW(8),
    MSG_SimulatorReady                   = MK_LOW(9), // SimAccess: U8, RegionFlags: U32, RegionID: LLUUID, EstateID: U32, ParentEstateID: U32, HasTelehub: BOOL, TelehubPos: LLVector3
    MSG_TelehubInfo                      = MK_LOW(10), // ObjectID: LLUUID, TelehubPos: LLVector3, TelehubRot: LLQuaternion, SpawnPointPos: LLVector3
    MSG_SimulatorPresentAtLocation       = MK_LOW(11), // Port: IPPORT, SimulatorIP: IPADDR, GridX: U32, GridY: U32, IP: IPADDR, Port: IPPORT, SimAccess: U8, RegionFlags: U32, RegionID: LLUUID, EstateID: U32, ParentEstateID: U32, HasTelehub: BOOL, TelehubPos: LLVector3
    MSG_SimulatorLoad                    = MK_LOW(12), // TimeDilation: F32, AgentCount: S32, CanAcceptAgents: BOOL, CircuitCode: U32, X: U8, Y: U8
    MSG_SimulatorShutdownRequest         = MK_LOW(13),
    MSG_RegionPresenceRequestByRegionID  = MK_LOW(14), // RegionID: LLUUID
    MSG_RegionPresenceRequestByHandle    = MK_LOW(15), // RegionHandle: U64
    MSG_RegionPresenceResponse           = MK_LOW(16), // RegionID: LLUUID, RegionHandle: U64, InternalRegionIP: IPADDR, ExternalRegionIP: IPADDR, RegionPort: IPPORT, ValidUntil: F64
    MSG_UpdateSimulator                  = MK_LOW(17), // RegionID: LLUUID, EstateID: U32, SimAccess: U8
    MSG_LogDwellTime                     = MK_LOW(18), // AgentID: LLUUID, SessionID: LLUUID, Duration: F32, RegionX: U32, RegionY: U32, AvgAgentsInView: U8, AvgViewerFPS: U8
    MSG_FeatureDisabled                  = MK_LOW(19), // AgentID: LLUUID, TransactionID: LLUUID
    MSG_LogFailedMoneyTransaction        = MK_LOW(20), // TransactionID: LLUUID, TransactionTime: U32, TransactionType: S32, SourceID: LLUUID, DestID: LLUUID, Flags: U8, Amount: S32, SimulatorIP: IPADDR, GridX: U32, GridY: U32, FailureType: U8
    MSG_UserReportInternal               = MK_LOW(21), // ReportType: U8, Category: U8, ReporterID: LLUUID, ViewerPosition: LLVector3, AgentPosition: LLVector3, ScreenshotID: LLUUID, ObjectID: LLUUID, OwnerID: LLUUID, LastOwnerID: LLUUID, CreatorID: LLUUID, RegionID: LLUUID, AbuserID: LLUUID, AbuseRegionID: LLUUID
    MSG_SetSimStatusInDatabase           = MK_LOW(22), // RegionID: LLUUID, X: S32, Y: S32, PID: S32, AgentCount: S32, TimeToLive: S32
    MSG_SetSimPresenceInDatabase         = MK_LOW(23), // RegionID: LLUUID, GridX: U32, GridY: U32, PID: S32, AgentCount: S32, TimeToLive: S32
    MSG_EconomyDataRequest               = MK_LOW(24),
    MSG_EconomyData                      = MK_LOW(25), // ObjectCapacity: S32, ObjectCount: S32, PriceEnergyUnit: S32, PriceObjectClaim: S32, PricePublicObjectDecay: S32, PricePublicObjectDelete: S32, PriceParcelClaim: S32, PriceParcelClaimFactor: F32, PriceUpload: S32, PriceRentLight: S32, TeleportMinPrice: S32, TeleportPriceExponent: F32, EnergyEfficiency: F32, PriceObjectRent: F32, PriceObjectScaleFactor: F32, PriceParcelRent: S32, PriceGroupCreate: S32
    MSG_AvatarPickerRequest              = MK_LOW(26), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID
    MSG_AvatarPickerRequestBackend       = MK_LOW(27), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, GodLevel: U8
    MSG_AvatarPickerReply                = MK_LOW(28), // AgentID: LLUUID, QueryID: LLUUID, AvatarID: LLUUID
    MSG_PlacesQuery                      = MK_LOW(29), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, TransactionID: LLUUID, QueryFlags: U32, Category: S8
    MSG_PlacesReply                      = MK_LOW(30), // AgentID: LLUUID, QueryID: LLUUID, TransactionID: LLUUID, OwnerID: LLUUID, ActualArea: S32, BillableArea: S32, Flags: U8, GlobalX: F32, GlobalY: F32, GlobalZ: F32, SnapshotID: LLUUID, Dwell: F32, Price: S32
    MSG_DirFindQuery                     = MK_LOW(31), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, QueryFlags: U32, QueryStart: S32
    MSG_DirFindQueryBackend              = MK_LOW(32), // AgentID: LLUUID, QueryID: LLUUID, QueryFlags: U32, QueryStart: S32, EstateID: U32, Godlike: BOOL
    MSG_DirPlacesQuery                   = MK_LOW(33), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, QueryFlags: U32, Category: S8, QueryStart: S32
    MSG_DirPlacesQueryBackend            = MK_LOW(34), // AgentID: LLUUID, QueryID: LLUUID, QueryFlags: U32, Category: S8, EstateID: U32, Godlike: BOOL, QueryStart: S32
    MSG_DirPlacesReply                   = MK_LOW(35), // AgentID: LLUUID, QueryID: LLUUID, ParcelID: LLUUID, ForSale: BOOL, Auction: BOOL, Dwell: F32, Status: U32
    MSG_DirPeopleReply                   = MK_LOW(36), // AgentID: LLUUID, QueryID: LLUUID, AgentID: LLUUID, Online: BOOL, Reputation: S32
    MSG_DirEventsReply                   = MK_LOW(37), // AgentID: LLUUID, QueryID: LLUUID, OwnerID: LLUUID, EventID: U32, UnixTime: U32, EventFlags: U32, Status: U32
    MSG_DirGroupsReply                   = MK_LOW(38), // AgentID: LLUUID, QueryID: LLUUID, GroupID: LLUUID, Members: S32, SearchOrder: F32
    MSG_DirClassifiedQuery               = MK_LOW(39), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, QueryFlags: U32, Category: U32, QueryStart: S32
    MSG_DirClassifiedQueryBackend        = MK_LOW(40), // AgentID: LLUUID, QueryID: LLUUID, QueryFlags: U32, Category: U32, EstateID: U32, Godlike: BOOL, QueryStart: S32
    MSG_DirClassifiedReply               = MK_LOW(41), // AgentID: LLUUID, QueryID: LLUUID, ClassifiedID: LLUUID, ClassifiedFlags: U8, CreationDate: U32, ExpirationDate: U32, PriceForListing: S32, Status: U32
    MSG_AvatarClassifiedReply            = MK_LOW(42), // AgentID: LLUUID, TargetID: LLUUID, ClassifiedID: LLUUID
    MSG_ClassifiedInfoRequest            = MK_LOW(43), // AgentID: LLUUID, SessionID: LLUUID, ClassifiedID: LLUUID
    MSG_ClassifiedInfoReply              = MK_LOW(44), // AgentID: LLUUID, ClassifiedID: LLUUID, CreatorID: LLUUID, CreationDate: U32, ExpirationDate: U32, Category: U32, ParcelID: LLUUID, ParentEstate: U32, SnapshotID: LLUUID, PosGlobal: LLVector3d, ClassifiedFlags: U8, PriceForListing: S32
    MSG_ClassifiedInfoUpdate             = MK_LOW(45), // AgentID: LLUUID, SessionID: LLUUID, ClassifiedID: LLUUID, Category: U32, ParcelID: LLUUID, ParentEstate: U32, SnapshotID: LLUUID, PosGlobal: LLVector3d, ClassifiedFlags: U8, PriceForListing: S32
    MSG_ClassifiedDelete                 = MK_LOW(46), // AgentID: LLUUID, SessionID: LLUUID, ClassifiedID: LLUUID
    MSG_ClassifiedGodDelete              = MK_LOW(47), // AgentID: LLUUID, SessionID: LLUUID, ClassifiedID: LLUUID, QueryID: LLUUID
    MSG_DirLandQuery                     = MK_LOW(48), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, QueryFlags: U32, SearchType: U32, Price: S32, Area: S32, QueryStart: S32
    MSG_DirLandQueryBackend              = MK_LOW(49), // AgentID: LLUUID, QueryID: LLUUID, QueryFlags: U32, SearchType: U32, Price: S32, Area: S32, QueryStart: S32, EstateID: U32, Godlike: BOOL
    MSG_DirLandReply                     = MK_LOW(50), // AgentID: LLUUID, QueryID: LLUUID, ParcelID: LLUUID, Auction: BOOL, ForSale: BOOL, SalePrice: S32, ActualArea: S32
    MSG_DirPopularQuery                  = MK_LOW(51), // AgentID: LLUUID, SessionID: LLUUID, QueryID: LLUUID, QueryFlags: U32
    MSG_DirPopularQueryBackend           = MK_LOW(52), // AgentID: LLUUID, QueryID: LLUUID, QueryFlags: U32, EstateID: U32, Godlike: BOOL
    MSG_DirPopularReply                  = MK_LOW(53), // AgentID: LLUUID, QueryID: LLUUID, ParcelID: LLUUID, Dwell: F32
    MSG_ParcelInfoRequest                = MK_LOW(54), // AgentID: LLUUID, SessionID: LLUUID, ParcelID: LLUUID
    MSG_ParcelInfoReply                  = MK_LOW(55), // AgentID: LLUUID, ParcelID: LLUUID, OwnerID: LLUUID, ActualArea: S32, BillableArea: S32, Flags: U8, GlobalX: F32, GlobalY: F32, GlobalZ: F32, SnapshotID: LLUUID, Dwell: F32, SalePrice: S32, AuctionID: S32
    MSG_ParcelObjectOwnersRequest        = MK_LOW(56), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32
    MSG_ParcelObjectOwnersReply          = MK_LOW(57), // OwnerID: LLUUID, IsGroupOwned: BOOL, Count: S32, OnlineStatus: BOOL
    MSG_GroupNoticesListRequest          = MK_LOW(58), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID
    MSG_GroupNoticesListReply            = MK_LOW(59), // AgentID: LLUUID, GroupID: LLUUID, NoticeID: LLUUID, Timestamp: U32, HasAttachment: BOOL, AssetType: U8
    MSG_GroupNoticeRequest               = MK_LOW(60), // AgentID: LLUUID, SessionID: LLUUID, GroupNoticeID: LLUUID
    MSG_GroupNoticeAdd                   = MK_LOW(61), // AgentID: LLUUID, ToGroupID: LLUUID, ID: LLUUID, Dialog: U8
    MSG_TeleportRequest                  = MK_LOW(62), // AgentID: LLUUID, SessionID: LLUUID, RegionID: LLUUID, Position: LLVector3, LookAt: LLVector3
    MSG_TeleportLocationRequest          = MK_LOW(63), // AgentID: LLUUID, SessionID: LLUUID, RegionHandle: U64, Position: LLVector3, LookAt: LLVector3
    MSG_TeleportLocal                    = MK_LOW(64), // AgentID: LLUUID, LocationID: U32, Position: LLVector3, LookAt: LLVector3, TeleportFlags: U32
    MSG_TeleportLandmarkRequest          = MK_LOW(65), // AgentID: LLUUID, SessionID: LLUUID, LandmarkID: LLUUID
    MSG_TeleportProgress                 = MK_LOW(66), // AgentID: LLUUID, TeleportFlags: U32
    MSG_DataHomeLocationRequest          = MK_LOW(67), // AgentID: LLUUID, KickedFromEstateID: U32, AgentEffectiveMaturity: U32
    MSG_DataHomeLocationReply            = MK_LOW(68), // AgentID: LLUUID, RegionHandle: U64, Position: LLVector3, LookAt: LLVector3
    MSG_TeleportFinish                   = MK_LOW(69), // AgentID: LLUUID, LocationID: U32, SimIP: IPADDR, SimPort: IPPORT, RegionHandle: U64, SimAccess: U8, TeleportFlags: U32
    MSG_StartLure                        = MK_LOW(70), // AgentID: LLUUID, SessionID: LLUUID, LureType: U8, TargetID: LLUUID
    MSG_TeleportLureRequest              = MK_LOW(71), // AgentID: LLUUID, SessionID: LLUUID, LureID: LLUUID, TeleportFlags: U32
    MSG_TeleportCancel                   = MK_LOW(72), // AgentID: LLUUID, SessionID: LLUUID
    MSG_TeleportStart                    = MK_LOW(73), // TeleportFlags: U32
    MSG_TeleportFailed                   = MK_LOW(74), // AgentID: LLUUID
    MSG_Undo                             = MK_LOW(75), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, ObjectID: LLUUID
    MSG_Redo                             = MK_LOW(76), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, ObjectID: LLUUID
    MSG_UndoLand                         = MK_LOW(77), // AgentID: LLUUID, SessionID: LLUUID
    MSG_AgentPause                       = MK_LOW(78), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: U32
    MSG_AgentResume                      = MK_LOW(79), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: U32
    MSG_AgentUpdate                      = MK_HIGH(4), // AgentID: LLUUID, SessionID: LLUUID, BodyRotation: LLQuaternion, HeadRotation: LLQuaternion, State: U8, CameraCenter: LLVector3, CameraAtAxis: LLVector3, CameraLeftAxis: LLVector3, CameraUpAxis: LLVector3, Far: F32, ControlFlags: U32, Flags: U8
    MSG_ChatFromViewer                   = MK_LOW(80), // AgentID: LLUUID, SessionID: LLUUID, Type: U8, Channel: S32
    MSG_AgentThrottle                    = MK_LOW(81), // AgentID: LLUUID, SessionID: LLUUID, CircuitCode: U32, GenCounter: U32
    MSG_AgentFOV                         = MK_LOW(82), // AgentID: LLUUID, SessionID: LLUUID, CircuitCode: U32, GenCounter: U32, VerticalAngle: F32
    MSG_AgentHeightWidth                 = MK_LOW(83), // AgentID: LLUUID, SessionID: LLUUID, CircuitCode: U32, GenCounter: U32, Height: U16, Width: U16
    MSG_AgentSetAppearance               = MK_LOW(84), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: U32, Size: LLVector3, CacheID: LLUUID, TextureIndex: U8, ParamValue: U8
    MSG_AgentAnimation                   = MK_HIGH(5), // AgentID: LLUUID, SessionID: LLUUID, AnimID: LLUUID, StartAnim: BOOL
    MSG_AgentRequestSit                  = MK_HIGH(6), // AgentID: LLUUID, SessionID: LLUUID, TargetID: LLUUID, Offset: LLVector3
    MSG_AgentSit                         = MK_HIGH(7), // AgentID: LLUUID, SessionID: LLUUID
    MSG_AgentQuitCopy                    = MK_LOW(85), // AgentID: LLUUID, SessionID: LLUUID, ViewerCircuitCode: U32
    MSG_RequestImage                     = MK_HIGH(8), // AgentID: LLUUID, SessionID: LLUUID, Image: LLUUID, DiscardLevel: S8, DownloadPriority: F32, Packet: U32, Type: U8
    MSG_ImageNotInDatabase               = MK_LOW(86), // ID: LLUUID
    MSG_RebakeAvatarTextures             = MK_LOW(87), // TextureID: LLUUID
    MSG_SetAlwaysRun                     = MK_LOW(88), // AgentID: LLUUID, SessionID: LLUUID, AlwaysRun: BOOL
    MSG_ObjectAdd                        = MK_MED(1), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, PCode: U8, Material: U8, AddFlags: U32, PathCurve: U8, ProfileCurve: U8, PathBegin: U16, PathEnd: U16, PathScaleX: U8, PathScaleY: U8, PathShearX: U8, PathShearY: U8, PathTwist: S8, PathTwistBegin: S8, PathRadiusOffset: S8, PathTaperX: S8, PathTaperY: S8, PathRevolutions: U8, PathSkew: S8, ProfileBegin: U16, ProfileEnd: U16, ProfileHollow: U16, BypassRaycast: U8, RayStart: LLVector3, RayEnd: LLVector3, RayTargetID: LLUUID, RayEndIsIntersection: U8, Scale: LLVector3, Rotation: LLQuaternion, State: U8
    MSG_ObjectDelete                     = MK_LOW(89), // AgentID: LLUUID, SessionID: LLUUID, Force: BOOL, ObjectLocalID: U32
    MSG_ObjectDuplicate                  = MK_LOW(90), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, Offset: LLVector3, DuplicateFlags: U32, ObjectLocalID: U32
    MSG_ObjectDuplicateOnRay             = MK_LOW(91), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RayStart: LLVector3, RayEnd: LLVector3, BypassRaycast: BOOL, RayEndIsIntersection: BOOL, CopyCenters: BOOL, CopyRotates: BOOL, RayTargetID: LLUUID, DuplicateFlags: U32, ObjectLocalID: U32
    MSG_MultipleObjectUpdate             = MK_MED(2), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, Type: U8
    MSG_RequestMultipleObjects           = MK_MED(3), // AgentID: LLUUID, SessionID: LLUUID, CacheMissType: U8, ID: U32
    MSG_ObjectPosition                   = MK_MED(4), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, Position: LLVector3
    MSG_ObjectScale                      = MK_LOW(92), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, Scale: LLVector3
    MSG_ObjectRotation                   = MK_LOW(93), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, Rotation: LLQuaternion
    MSG_ObjectFlagUpdate                 = MK_LOW(94), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, UsePhysics: BOOL, IsTemporary: BOOL, IsPhantom: BOOL, CastsShadows: BOOL, PhysicsShapeType: U8, Density: F32, Friction: F32, Restitution: F32, GravityMultiplier: F32
    MSG_ObjectClickAction                = MK_LOW(95), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, ClickAction: U8
    MSG_ObjectImage                      = MK_LOW(96), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectBypassModUpdate            = MK_LOW(431), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, PropertyID: U8
    MSG_ObjectMaterial                   = MK_LOW(97), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, Material: U8
    MSG_ObjectShape                      = MK_LOW(98), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, PathCurve: U8, ProfileCurve: U8, PathBegin: U16, PathEnd: U16, PathScaleX: U8, PathScaleY: U8, PathShearX: U8, PathShearY: U8, PathTwist: S8, PathTwistBegin: S8, PathRadiusOffset: S8, PathTaperX: S8, PathTaperY: S8, PathRevolutions: U8, PathSkew: S8, ProfileBegin: U16, ProfileEnd: U16, ProfileHollow: U16
    MSG_ObjectExtraParams                = MK_LOW(99), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, ParamType: U16, ParamInUse: BOOL, ParamSize: U32
    MSG_ObjectOwner                      = MK_LOW(100), // AgentID: LLUUID, SessionID: LLUUID, Override: BOOL, OwnerID: LLUUID, GroupID: LLUUID, ObjectLocalID: U32
    MSG_ObjectGroup                      = MK_LOW(101), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, ObjectLocalID: U32
    MSG_ObjectBuy                        = MK_LOW(102), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, CategoryID: LLUUID, ObjectLocalID: U32, SaleType: U8, SalePrice: S32
    MSG_BuyObjectInventory               = MK_LOW(103), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, ItemID: LLUUID, FolderID: LLUUID
    MSG_DerezContainer                   = MK_LOW(104), // ObjectID: LLUUID, Delete: BOOL
    MSG_ObjectPermissions                = MK_LOW(105), // AgentID: LLUUID, SessionID: LLUUID, Override: BOOL, ObjectLocalID: U32, Field: U8, Set: U8, Mask: U32
    MSG_ObjectSaleInfo                   = MK_LOW(106), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, SaleType: U8, SalePrice: S32
    MSG_ObjectName                       = MK_LOW(107), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32
    MSG_ObjectDescription                = MK_LOW(108), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32
    MSG_ObjectCategory                   = MK_LOW(109), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, Category: U32
    MSG_ObjectSelect                     = MK_LOW(110), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectDeselect                   = MK_LOW(111), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectAttach                     = MK_LOW(112), // AgentID: LLUUID, SessionID: LLUUID, AttachmentPoint: U8, ObjectLocalID: U32, Rotation: LLQuaternion
    MSG_ObjectDetach                     = MK_LOW(113), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectDrop                       = MK_LOW(114), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectLink                       = MK_LOW(115), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectDelink                     = MK_LOW(116), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32
    MSG_ObjectGrab                       = MK_LOW(117), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, GrabOffset: LLVector3, UVCoord: LLVector3, STCoord: LLVector3, FaceIndex: S32, Position: LLVector3, Normal: LLVector3, Binormal: LLVector3
    MSG_ObjectGrabUpdate                 = MK_LOW(118), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, GrabOffsetInitial: LLVector3, GrabPosition: LLVector3, TimeSinceLast: U32, UVCoord: LLVector3, STCoord: LLVector3, FaceIndex: S32, Position: LLVector3, Normal: LLVector3, Binormal: LLVector3
    MSG_ObjectDeGrab                     = MK_LOW(119), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, UVCoord: LLVector3, STCoord: LLVector3, FaceIndex: S32, Position: LLVector3, Normal: LLVector3, Binormal: LLVector3
    MSG_ObjectSpinStart                  = MK_LOW(120), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID
    MSG_ObjectSpinUpdate                 = MK_LOW(121), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, Rotation: LLQuaternion
    MSG_ObjectSpinStop                   = MK_LOW(122), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID
    MSG_ObjectExportSelected             = MK_LOW(123), // AgentID: LLUUID, RequestID: LLUUID, VolumeDetail: S16, ObjectID: LLUUID
    MSG_ModifyLand                       = MK_LOW(124), // AgentID: LLUUID, SessionID: LLUUID, Action: U8, BrushSize: U8, Seconds: F32, Height: F32, LocalID: S32, West: F32, South: F32, East: F32, North: F32, BrushSize: F32
    MSG_VelocityInterpolateOn            = MK_LOW(125), // AgentID: LLUUID, SessionID: LLUUID
    MSG_VelocityInterpolateOff           = MK_LOW(126), // AgentID: LLUUID, SessionID: LLUUID
    MSG_StateSave                        = MK_LOW(127), // AgentID: LLUUID, SessionID: LLUUID
    MSG_ReportAutosaveCrash              = MK_LOW(128), // PID: S32, Status: S32
    MSG_SimWideDeletes                   = MK_LOW(129), // AgentID: LLUUID, SessionID: LLUUID, TargetID: LLUUID, Flags: U32
    MSG_RequestObjectPropertiesFamily    = MK_MED(5), // AgentID: LLUUID, SessionID: LLUUID, RequestFlags: U32, ObjectID: LLUUID
    MSG_TrackAgent                       = MK_LOW(130), // AgentID: LLUUID, SessionID: LLUUID, PreyID: LLUUID
    MSG_ViewerStats                      = MK_LOW(131), // AgentID: LLUUID, SessionID: LLUUID, IP: IPADDR, StartTime: U32, RunTime: F32, SimFPS: F32, FPS: F32, AgentsInView: U8, Ping: F32, MetersTraveled: F64, RegionsVisited: S32, SysRAM: U32, World: U32, Objects: U32, Textures: U32, Bytes: U32, Packets: U32, Compressed: U32, Savings: U32, SendPacket: U32, Dropped: U32, Resent: U32, FailedResends: U32, OffCircuit: U32, Invalid: U32, Type: U32, Value: F64
    MSG_ScriptAnswerYes                  = MK_LOW(132), // AgentID: LLUUID, SessionID: LLUUID, TaskID: LLUUID, ItemID: LLUUID, Questions: S32
    MSG_UserReport                       = MK_LOW(133), // AgentID: LLUUID, SessionID: LLUUID, ReportType: U8, Category: U8, Position: LLVector3, CheckFlags: U8, ScreenshotID: LLUUID, ObjectID: LLUUID, AbuserID: LLUUID, AbuseRegionID: LLUUID
    MSG_AlertMessage                     = MK_LOW(134), // AgentID: LLUUID
    MSG_AgentAlertMessage                = MK_LOW(135), // AgentID: LLUUID, Modal: BOOL
    MSG_MeanCollisionAlert               = MK_LOW(136), // Victim: LLUUID, Perp: LLUUID, Time: U32, Mag: F32, Type: U8
    MSG_ViewerFrozenMessage              = MK_LOW(137), // Data: BOOL
    MSG_HealthMessage                    = MK_LOW(138), // Health: F32
    MSG_ChatFromSimulator                = MK_LOW(139), // SourceID: LLUUID, OwnerID: LLUUID, SourceType: U8, ChatType: U8, Audible: U8, Position: LLVector3
    MSG_SimStats                         = MK_LOW(140), // RegionX: U32, RegionY: U32, RegionFlags: U32, ObjectCapacity: U32, StatID: U32, StatValue: F32, PID: S32, RegionFlagsExtended: U64
    MSG_RequestRegionInfo                = MK_LOW(141), // AgentID: LLUUID, SessionID: LLUUID
    MSG_RegionInfo                       = MK_LOW(142), // AgentID: LLUUID, SessionID: LLUUID, EstateID: U32, ParentEstateID: U32, RegionFlags: U32, SimAccess: U8, MaxAgents: U8, BillableFactor: F32, ObjectBonusFactor: F32, WaterHeight: F32, TerrainRaiseLimit: F32, TerrainLowerLimit: F32, PricePerMeter: S32, RedirectGridX: S32, RedirectGridY: S32, UseEstateSun: BOOL, SunHour: F32, MaxAgents32: U32, HardMaxAgents: U32, HardMaxObjects: U32, RegionFlagsExtended: U64, ChatWhisperRange: F32, ChatNormalRange: F32, ChatShoutRange: F32, ChatWhisperOffset: F32, ChatNormalOffset: F32, ChatShoutOffset: F32, ChatFlags: U32, CombatFlags: U32, OnDeath: U8, DamageThrottle: F32, RegenerationRate: F32, InvulnerabilyTime: F32, DamageLimit: F32
    MSG_GodUpdateRegionInfo              = MK_LOW(143), // AgentID: LLUUID, SessionID: LLUUID, EstateID: U32, ParentEstateID: U32, RegionFlags: U32, BillableFactor: F32, PricePerMeter: S32, RedirectGridX: S32, RedirectGridY: S32, RegionFlagsExtended: U64
    MSG_NearestLandingRegionRequest      = MK_LOW(144), // RegionHandle: U64
    MSG_NearestLandingRegionReply        = MK_LOW(145), // RegionHandle: U64
    MSG_NearestLandingRegionUpdated      = MK_LOW(146), // RegionHandle: U64
    MSG_TeleportLandingStatusChanged     = MK_LOW(147), // RegionHandle: U64
    MSG_RegionHandshake                  = MK_LOW(148), // RegionFlags: U32, SimAccess: U8, SimOwner: LLUUID, IsEstateManager: BOOL, WaterHeight: F32, BillableFactor: F32, CacheID: LLUUID, TerrainBase0: LLUUID, TerrainBase1: LLUUID, TerrainBase2: LLUUID, TerrainBase3: LLUUID, TerrainDetail0: LLUUID, TerrainDetail1: LLUUID, TerrainDetail2: LLUUID, TerrainDetail3: LLUUID, TerrainStartHeight00: F32, TerrainStartHeight01: F32, TerrainStartHeight10: F32, TerrainStartHeight11: F32, TerrainHeightRange00: F32, TerrainHeightRange01: F32, TerrainHeightRange10: F32, TerrainHeightRange11: F32, RegionID: LLUUID, CPUClassID: S32, CPURatio: S32, RegionFlagsExtended: U64, RegionProtocols: U64
    MSG_RegionHandshakeReply             = MK_LOW(149), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32
    MSG_CoarseLocationUpdate             = MK_MED(6), // X: U8, Y: U8, Z: U8, You: S16, Prey: S16, AgentID: LLUUID
    MSG_ImageData                        = MK_HIGH(9), // ID: LLUUID, Codec: U8, Size: U32, Packets: U16
    MSG_ImagePacket                      = MK_HIGH(10), // ID: LLUUID, Packet: U16
    MSG_LayerData                        = MK_HIGH(11), // Type: U8
    MSG_ObjectUpdate                     = MK_HIGH(12), // RegionHandle: U64, TimeDilation: U16, ID: U32, State: U8, FullID: LLUUID, CRC: U32, PCode: U8, Material: U8, ClickAction: U8, Scale: LLVector3, ParentID: U32, UpdateFlags: U32, PathCurve: U8, ProfileCurve: U8, PathBegin: U16, PathEnd: U16, PathScaleX: U8, PathScaleY: U8, PathShearX: U8, PathShearY: U8, PathTwist: S8, PathTwistBegin: S8, PathRadiusOffset: S8, PathTaperX: S8, PathTaperY: S8, PathRevolutions: U8, PathSkew: S8, ProfileBegin: U16, ProfileEnd: U16, ProfileHollow: U16, Sound: LLUUID, OwnerID: LLUUID, Gain: F32, Flags: U8, Radius: F32, JointType: U8, JointPivot: LLVector3, JointAxisOrAnchor: LLVector3
    MSG_ObjectUpdateCompressed           = MK_HIGH(13), // RegionHandle: U64, TimeDilation: U16, UpdateFlags: U32
    MSG_ObjectUpdateCached               = MK_HIGH(14), // RegionHandle: U64, TimeDilation: U16, ID: U32, CRC: U32, UpdateFlags: U32
    MSG_ImprovedTerseObjectUpdate        = MK_HIGH(15), // RegionHandle: U64, TimeDilation: U16
    MSG_KillObject                       = MK_HIGH(16), // ID: U32
    MSG_CrossedRegion                    = MK_MED(7), // AgentID: LLUUID, SessionID: LLUUID, SimIP: IPADDR, SimPort: IPPORT, RegionHandle: U64, Position: LLVector3, LookAt: LLVector3
    MSG_SimulatorViewerTimeMessage       = MK_LOW(150), // UsecSinceStart: U64, SecPerDay: U32, SecPerYear: U32, SunDirection: LLVector3, SunPhase: F32, SunAngVelocity: LLVector3
    MSG_EnableSimulator                  = MK_LOW(151), // Handle: U64, IP: IPADDR, Port: IPPORT
    MSG_DisableSimulator                 = MK_LOW(152),
    MSG_ConfirmEnableSimulator           = MK_MED(8), // AgentID: LLUUID, SessionID: LLUUID
    MSG_TransferRequest                  = MK_LOW(153), // TransferID: LLUUID, ChannelType: S32, SourceType: S32, Priority: F32
    MSG_TransferInfo                     = MK_LOW(154), // TransferID: LLUUID, ChannelType: S32, TargetType: S32, Status: S32, Size: S32
    MSG_TransferPacket                   = MK_HIGH(17), // TransferID: LLUUID, ChannelType: S32, Packet: S32, Status: S32
    MSG_TransferAbort                    = MK_LOW(155), // TransferID: LLUUID, ChannelType: S32
    MSG_RequestXfer                      = MK_LOW(156), // ID: U64, FilePath: U8, DeleteOnCompletion: BOOL, UseBigPackets: BOOL, VFileID: LLUUID, VFileType: S16
    MSG_SendXferPacket                   = MK_HIGH(18), // ID: U64, Packet: U32
    MSG_ConfirmXferPacket                = MK_HIGH(19), // ID: U64, Packet: U32
    MSG_AbortXfer                        = MK_LOW(157), // ID: U64, Result: S32
    MSG_AvatarAnimation                  = MK_HIGH(20), // ID: LLUUID, AnimID: LLUUID, AnimSequenceID: S32, ObjectID: LLUUID
    MSG_AvatarAppearance                 = MK_LOW(158), // ID: LLUUID, IsTrial: BOOL, ParamValue: U8, AppearanceVersion: U8, CofVersion: S32, Flags: U32, HoverHeight: LLVector3, ID: LLUUID, AttachmentPoint: U8
    MSG_AvatarSitResponse                = MK_HIGH(21), // ID: LLUUID, AutoPilot: BOOL, SitPosition: LLVector3, SitRotation: LLQuaternion, CameraEyeOffset: LLVector3, CameraAtOffset: LLVector3, ForceMouselook: BOOL
    MSG_SetFollowCamProperties           = MK_LOW(159), // ObjectID: LLUUID, Type: S32, Value: F32
    MSG_ClearFollowCamProperties         = MK_LOW(160), // ObjectID: LLUUID
    MSG_CameraConstraint                 = MK_HIGH(22), // Plane: LLVector4
    MSG_ObjectProperties                 = MK_MED(9), // ObjectID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, CreationDate: U64, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, OwnershipCost: S32, SaleType: U8, SalePrice: S32, AggregatePerms: U8, AggregatePermTextures: U8, AggregatePermTexturesOwner: U8, Category: U32, InventorySerial: S16, ItemID: LLUUID, FolderID: LLUUID, FromTaskID: LLUUID, LastOwnerID: LLUUID
    MSG_ObjectPropertiesFamily           = MK_MED(10), // RequestFlags: U32, ObjectID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, OwnershipCost: S32, SaleType: U8, SalePrice: S32, Category: U32, LastOwnerID: LLUUID
    MSG_RequestPayPrice                  = MK_LOW(161), // ObjectID: LLUUID
    MSG_PayPriceReply                    = MK_LOW(162), // ObjectID: LLUUID, DefaultPayPrice: S32, PayButton: S32
    MSG_KickUser                         = MK_LOW(163), // TargetIP: IPADDR, TargetPort: IPPORT, AgentID: LLUUID, SessionID: LLUUID
    MSG_KickUserAck                      = MK_LOW(164), // SessionID: LLUUID, Flags: U32
    MSG_GodKickUser                      = MK_LOW(165), // GodID: LLUUID, GodSessionID: LLUUID, AgentID: LLUUID, KickFlags: U32
    MSG_SystemKickUser                   = MK_LOW(166), // AgentID: LLUUID
    MSG_EjectUser                        = MK_LOW(167), // AgentID: LLUUID, SessionID: LLUUID, TargetID: LLUUID, Flags: U32
    MSG_FreezeUser                       = MK_LOW(168), // AgentID: LLUUID, SessionID: LLUUID, TargetID: LLUUID, Flags: U32
    MSG_AvatarPropertiesRequest          = MK_LOW(169), // AgentID: LLUUID, SessionID: LLUUID, AvatarID: LLUUID
    MSG_AvatarPropertiesRequestBackend   = MK_LOW(170), // AgentID: LLUUID, AvatarID: LLUUID, GodLevel: U8, WebProfilesDisabled: BOOL
    MSG_AvatarPropertiesReply            = MK_LOW(171), // AgentID: LLUUID, AvatarID: LLUUID, ImageID: LLUUID, FLImageID: LLUUID, PartnerID: LLUUID, Flags: U32
    MSG_AvatarInterestsReply             = MK_LOW(172), // AgentID: LLUUID, AvatarID: LLUUID, WantToMask: U32, SkillsMask: U32
    MSG_AvatarGroupsReply                = MK_LOW(173), // AgentID: LLUUID, AvatarID: LLUUID, GroupPowers: U64, AcceptNotices: BOOL, GroupID: LLUUID, GroupInsigniaID: LLUUID, ListInProfile: BOOL
    MSG_AvatarPropertiesUpdate           = MK_LOW(174), // AgentID: LLUUID, SessionID: LLUUID, ImageID: LLUUID, FLImageID: LLUUID, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_AvatarInterestsUpdate            = MK_LOW(175), // AgentID: LLUUID, SessionID: LLUUID, WantToMask: U32, SkillsMask: U32
    MSG_AvatarNotesReply                 = MK_LOW(176), // AgentID: LLUUID, TargetID: LLUUID
    MSG_AvatarNotesUpdate                = MK_LOW(177), // AgentID: LLUUID, SessionID: LLUUID, TargetID: LLUUID
    MSG_AvatarPicksReply                 = MK_LOW(178), // AgentID: LLUUID, TargetID: LLUUID, PickID: LLUUID
    MSG_EventInfoRequest                 = MK_LOW(179), // AgentID: LLUUID, SessionID: LLUUID, EventID: U32
    MSG_EventInfoReply                   = MK_LOW(180), // AgentID: LLUUID, EventID: U32, DateUTC: U32, Duration: U32, Cover: U32, Amount: U32, GlobalPos: LLVector3d, EventFlags: U32
    MSG_EventNotificationAddRequest      = MK_LOW(181), // AgentID: LLUUID, SessionID: LLUUID, EventID: U32
    MSG_EventNotificationRemoveRequest   = MK_LOW(182), // AgentID: LLUUID, SessionID: LLUUID, EventID: U32
    MSG_EventGodDelete                   = MK_LOW(183), // AgentID: LLUUID, SessionID: LLUUID, EventID: U32, QueryID: LLUUID, QueryFlags: U32, QueryStart: S32
    MSG_PickInfoReply                    = MK_LOW(184), // AgentID: LLUUID, PickID: LLUUID, CreatorID: LLUUID, TopPick: BOOL, ParcelID: LLUUID, SnapshotID: LLUUID, PosGlobal: LLVector3d, SortOrder: S32, Enabled: BOOL
    MSG_PickInfoUpdate                   = MK_LOW(185), // AgentID: LLUUID, SessionID: LLUUID, PickID: LLUUID, CreatorID: LLUUID, TopPick: BOOL, ParcelID: LLUUID, SnapshotID: LLUUID, PosGlobal: LLVector3d, SortOrder: S32, Enabled: BOOL
    MSG_PickDelete                       = MK_LOW(186), // AgentID: LLUUID, SessionID: LLUUID, PickID: LLUUID
    MSG_PickGodDelete                    = MK_LOW(187), // AgentID: LLUUID, SessionID: LLUUID, PickID: LLUUID, QueryID: LLUUID
    MSG_ScriptQuestion                   = MK_LOW(188), // TaskID: LLUUID, ItemID: LLUUID, Questions: S32, ExperienceID: LLUUID
    MSG_ScriptControlChange              = MK_LOW(189), // TakeControls: BOOL, Controls: U32, PassToAgent: BOOL
    MSG_ScriptDialog                     = MK_LOW(190), // ObjectID: LLUUID, ChatChannel: S32, ImageID: LLUUID, OwnerID: LLUUID
    MSG_ScriptDialogReply                = MK_LOW(191), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, ChatChannel: S32, ButtonIndex: S32
    MSG_ForceScriptControlRelease        = MK_LOW(192), // AgentID: LLUUID, SessionID: LLUUID
    MSG_RevokePermissions                = MK_LOW(193), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, ObjectPermissions: U32
    MSG_LoadURL                          = MK_LOW(194), // ObjectID: LLUUID, OwnerID: LLUUID, OwnerIsGroup: BOOL
    MSG_ScriptTeleportRequest            = MK_LOW(195), // SimPosition: LLVector3, LookAt: LLVector3, Flags: U32
    MSG_ParcelOverlay                    = MK_LOW(196), // SequenceID: S32
    MSG_ParcelPropertiesRequest          = MK_MED(11), // AgentID: LLUUID, SessionID: LLUUID, SequenceID: S32, West: F32, South: F32, East: F32, North: F32, SnapSelection: BOOL
    MSG_ParcelPropertiesRequestByID      = MK_LOW(197), // AgentID: LLUUID, SessionID: LLUUID, SequenceID: S32, LocalID: S32
    MSG_ParcelProperties                 = MK_HIGH(23), // RequestResult: S32, SequenceID: S32, SnapSelection: BOOL, SelfCount: S32, OtherCount: S32, PublicCount: S32, LocalID: S32, OwnerID: LLUUID, IsGroupOwned: BOOL, AuctionID: U32, ClaimDate: S32, ClaimPrice: S32, RentPrice: S32, AABBMin: LLVector3, AABBMax: LLVector3, Area: S32, Status: U8, SimWideMaxPrims: S32, SimWideTotalPrims: S32, MaxPrims: S32, TotalPrims: S32, OwnerPrims: S32, GroupPrims: S32, OtherPrims: S32, SelectedPrims: S32, ParcelPrimBonus: F32, OtherCleanTime: S32, ParcelFlags: U32, SalePrice: S32, MediaID: LLUUID, MediaAutoScale: U8, GroupID: LLUUID, PassPrice: S32, PassHours: F32, Category: U8, AuthBuyerID: LLUUID, SnapshotID: LLUUID, UserLocation: LLVector3, UserLookAt: LLVector3, LandingType: U8, RegionPushOverride: BOOL, RegionDenyAnonymous: BOOL, RegionDenyIdentified: BOOL, RegionDenyTransacted: BOOL, RegionDenyAgeUnverified: BOOL, RegionAllowAccessOverride: BOOL, ParcelEnvironmentVersion: S32, RegionAllowEnvironmentOverride: BOOL
    MSG_ParcelPropertiesUpdate           = MK_LOW(198), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, Flags: U32, ParcelFlags: U32, SalePrice: S32, MediaID: LLUUID, MediaAutoScale: U8, GroupID: LLUUID, PassPrice: S32, PassHours: F32, Category: U8, AuthBuyerID: LLUUID, SnapshotID: LLUUID, UserLocation: LLVector3, UserLookAt: LLVector3, LandingType: U8
    MSG_ParcelReturnObjects              = MK_LOW(199), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, ReturnType: U32, TaskID: LLUUID, OwnerID: LLUUID
    MSG_ParcelSetOtherCleanTime          = MK_LOW(200), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, OtherCleanTime: S32
    MSG_ParcelDisableObjects             = MK_LOW(201), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, ReturnType: U32, TaskID: LLUUID, OwnerID: LLUUID
    MSG_ParcelSelectObjects              = MK_LOW(202), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, ReturnType: U32, ReturnID: LLUUID
    MSG_EstateCovenantRequest            = MK_LOW(203), // AgentID: LLUUID, SessionID: LLUUID
    MSG_EstateCovenantReply              = MK_LOW(204), // CovenantID: LLUUID, CovenantTimestamp: U32, EstateOwnerID: LLUUID
    MSG_ForceObjectSelect                = MK_LOW(205), // ResetList: BOOL, LocalID: U32
    MSG_ParcelBuyPass                    = MK_LOW(206), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32
    MSG_ParcelDeedToGroup                = MK_LOW(207), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, LocalID: S32
    MSG_ParcelReclaim                    = MK_LOW(208), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32
    MSG_ParcelClaim                      = MK_LOW(209), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, IsGroupOwned: BOOL, Final: BOOL, West: F32, South: F32, East: F32, North: F32
    MSG_ParcelJoin                       = MK_LOW(210), // AgentID: LLUUID, SessionID: LLUUID, West: F32, South: F32, East: F32, North: F32
    MSG_ParcelDivide                     = MK_LOW(211), // AgentID: LLUUID, SessionID: LLUUID, West: F32, South: F32, East: F32, North: F32
    MSG_ParcelRelease                    = MK_LOW(212), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32
    MSG_ParcelBuy                        = MK_LOW(213), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, IsGroupOwned: BOOL, RemoveContribution: BOOL, LocalID: S32, Final: BOOL, Price: S32, Area: S32
    MSG_ParcelGodForceOwner              = MK_LOW(214), // AgentID: LLUUID, SessionID: LLUUID, OwnerID: LLUUID, LocalID: S32
    MSG_ParcelAccessListRequest          = MK_LOW(215), // AgentID: LLUUID, SessionID: LLUUID, SequenceID: S32, Flags: U32, LocalID: S32
    MSG_ParcelAccessListReply            = MK_LOW(216), // AgentID: LLUUID, SequenceID: S32, Flags: U32, LocalID: S32, ID: LLUUID, Time: S32, Flags: U32
    MSG_ParcelAccessListUpdate           = MK_LOW(217), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, LocalID: S32, TransactionID: LLUUID, SequenceID: S32, Sections: S32, ID: LLUUID, Time: S32, Flags: U32
    MSG_ParcelDwellRequest               = MK_LOW(218), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, ParcelID: LLUUID
    MSG_ParcelDwellReply                 = MK_LOW(219), // AgentID: LLUUID, LocalID: S32, ParcelID: LLUUID, Dwell: F32
    MSG_RequestParcelTransfer            = MK_LOW(220), // TransactionID: LLUUID, TransactionTime: U32, SourceID: LLUUID, DestID: LLUUID, OwnerID: LLUUID, Flags: U8, TransactionType: S32, Amount: S32, BillableArea: S32, ActualArea: S32, Final: BOOL, RegionID: LLUUID, GridX: U32, GridY: U32
    MSG_UpdateParcel                     = MK_LOW(221), // ParcelID: LLUUID, RegionHandle: U64, OwnerID: LLUUID, GroupOwned: BOOL, Status: U8, RegionX: F32, RegionY: F32, ActualArea: S32, BillableArea: S32, ShowDir: BOOL, IsForSale: BOOL, Category: U8, SnapshotID: LLUUID, UserLocation: LLVector3, SalePrice: S32, AuthorizedBuyerID: LLUUID, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_RemoveParcel                     = MK_LOW(222), // ParcelID: LLUUID
    MSG_MergeParcel                      = MK_LOW(223), // MasterID: LLUUID, SlaveID: LLUUID
    MSG_LogParcelChanges                 = MK_LOW(224), // AgentID: LLUUID, RegionHandle: U64, ParcelID: LLUUID, OwnerID: LLUUID, IsOwnerGroup: BOOL, ActualArea: S32, Action: S8, TransactionID: LLUUID
    MSG_CheckParcelSales                 = MK_LOW(225), // RegionHandle: U64
    MSG_ParcelSales                      = MK_LOW(226), // ParcelID: LLUUID, BuyerID: LLUUID
    MSG_ParcelGodMarkAsContent           = MK_LOW(227), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32
    MSG_ViewerStartAuction               = MK_LOW(228), // AgentID: LLUUID, SessionID: LLUUID, LocalID: S32, SnapshotID: LLUUID
    MSG_StartAuction                     = MK_LOW(229), // AgentID: LLUUID, ParcelID: LLUUID, SnapshotID: LLUUID
    MSG_ConfirmAuctionStart              = MK_LOW(230), // ParcelID: LLUUID, AuctionID: U32
    MSG_CompleteAuction                  = MK_LOW(231), // ParcelID: LLUUID
    MSG_CancelAuction                    = MK_LOW(232), // ParcelID: LLUUID
    MSG_CheckParcelAuctions              = MK_LOW(233), // RegionHandle: U64
    MSG_ParcelAuctions                   = MK_LOW(234), // ParcelID: LLUUID, WinnerID: LLUUID
    MSG_UUIDNameRequest                  = MK_LOW(235), // ID: LLUUID
    MSG_UUIDNameReply                    = MK_LOW(236), // ID: LLUUID
    MSG_UUIDGroupNameRequest             = MK_LOW(237), // ID: LLUUID
    MSG_UUIDGroupNameReply               = MK_LOW(238), // ID: LLUUID
    MSG_ChatPass                         = MK_LOW(239), // Channel: S32, Position: LLVector3, ID: LLUUID, OwnerID: LLUUID, SourceType: U8, Type: U8, Radius: F32, SimAccess: U8
    MSG_EdgeDataPacket                   = MK_HIGH(24), // LayerType: U8, Direction: U8
    MSG_SimStatus                        = MK_MED(12), // CanAcceptAgents: BOOL, CanAcceptTasks: BOOL, Flags: U64
    MSG_ChildAgentUpdate                 = MK_HIGH(25), // RegionHandle: U64, ViewerCircuitCode: U32, AgentID: LLUUID, SessionID: LLUUID, AgentPos: LLVector3, AgentVel: LLVector3, Center: LLVector3, Size: LLVector3, AtAxis: LLVector3, LeftAxis: LLVector3, UpAxis: LLVector3, ChangedGrid: BOOL, Far: F32, Aspect: F32, LocomotionState: U32, HeadRotation: LLQuaternion, BodyRotation: LLQuaternion, ControlFlags: U32, EnergyLevel: F32, GodLevel: U8, AlwaysRun: BOOL, PreyAgent: LLUUID, AgentAccess: U8, ActiveGroupID: LLUUID, GroupID: LLUUID, GroupPowers: U64, AcceptNotices: BOOL, Animation: LLUUID, ObjectID: LLUUID, GranterID: LLUUID, ParamValue: U8, AgentLegacyAccess: U8, AgentMaxAccess: U8, Flags: U32
    MSG_ChildAgentAlive                  = MK_HIGH(26), // RegionHandle: U64, ViewerCircuitCode: U32, AgentID: LLUUID, SessionID: LLUUID
    MSG_ChildAgentPositionUpdate         = MK_HIGH(27), // RegionHandle: U64, ViewerCircuitCode: U32, AgentID: LLUUID, SessionID: LLUUID, AgentPos: LLVector3, AgentVel: LLVector3, Center: LLVector3, Size: LLVector3, AtAxis: LLVector3, LeftAxis: LLVector3, UpAxis: LLVector3, ChangedGrid: BOOL
    MSG_ChildAgentDying                  = MK_LOW(240), // AgentID: LLUUID, SessionID: LLUUID
    MSG_ChildAgentUnknown                = MK_LOW(241), // AgentID: LLUUID, SessionID: LLUUID
    MSG_AtomicPassObject                 = MK_HIGH(28), // TaskID: LLUUID, AttachmentNeedsSave: BOOL
    MSG_KillChildAgents                  = MK_LOW(242), // AgentID: LLUUID
    MSG_GetScriptRunning                 = MK_LOW(243), // ObjectID: LLUUID, ItemID: LLUUID
    MSG_ScriptRunningReply               = MK_LOW(244), // ObjectID: LLUUID, ItemID: LLUUID, Running: BOOL
    MSG_SetScriptRunning                 = MK_LOW(245), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, ItemID: LLUUID, Running: BOOL
    MSG_ScriptReset                      = MK_LOW(246), // AgentID: LLUUID, SessionID: LLUUID, ObjectID: LLUUID, ItemID: LLUUID
    MSG_ScriptSensorRequest              = MK_LOW(247), // SourceID: LLUUID, RequestID: LLUUID, SearchID: LLUUID, SearchPos: LLVector3, SearchDir: LLQuaternion, Type: S32, Range: F32, Arc: F32, RegionHandle: U64, SearchRegions: U8
    MSG_ScriptSensorReply                = MK_LOW(248), // SourceID: LLUUID, ObjectID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, Position: LLVector3, Velocity: LLVector3, Rotation: LLQuaternion, Type: S32, Range: F32
    MSG_CompleteAgentMovement            = MK_LOW(249), // AgentID: LLUUID, SessionID: LLUUID, CircuitCode: U32
    MSG_AgentMovementComplete            = MK_LOW(250), // AgentID: LLUUID, SessionID: LLUUID, Position: LLVector3, LookAt: LLVector3, RegionHandle: U64, Timestamp: U32
    MSG_DataServerLogout                 = MK_LOW(251), // AgentID: LLUUID, ViewerIP: IPADDR, Disconnect: BOOL, SessionID: LLUUID
    MSG_LogoutRequest                    = MK_LOW(252), // AgentID: LLUUID, SessionID: LLUUID
    MSG_LogoutReply                      = MK_LOW(253), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID
    MSG_ImprovedInstantMessage           = MK_LOW(254), // AgentID: LLUUID, SessionID: LLUUID, FromGroup: BOOL, ToAgentID: LLUUID, ParentEstateID: U32, RegionID: LLUUID, Position: LLVector3, Offline: U8, Dialog: U8, ID: LLUUID, Timestamp: U32, EstateID: U32
    MSG_RetrieveInstantMessages          = MK_LOW(255), // AgentID: LLUUID, SessionID: LLUUID
    MSG_FindAgent                        = MK_LOW(256), // Hunter: LLUUID, Prey: LLUUID, SpaceIP: IPADDR, GlobalX: F64, GlobalY: F64
    MSG_RequestGodlikePowers             = MK_LOW(257), // AgentID: LLUUID, SessionID: LLUUID, Godlike: BOOL, Token: LLUUID
    MSG_GrantGodlikePowers               = MK_LOW(258), // AgentID: LLUUID, SessionID: LLUUID, GodLevel: U8, Token: LLUUID
    MSG_GodlikeMessage                   = MK_LOW(259), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, Invoice: LLUUID
    MSG_EstateOwnerMessage               = MK_LOW(260), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, Invoice: LLUUID
    MSG_GenericMessage                   = MK_LOW(261), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, Invoice: LLUUID
    MSG_GenericStreamingMessage          = MK_HIGH(31), // Method: U16
    MSG_LargeGenericMessage              = MK_LOW(430), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, Invoice: LLUUID
    MSG_MuteListRequest                  = MK_LOW(262), // AgentID: LLUUID, SessionID: LLUUID, MuteCRC: U32
    MSG_UpdateMuteListEntry              = MK_LOW(263), // AgentID: LLUUID, SessionID: LLUUID, MuteID: LLUUID, MuteType: S32, MuteFlags: U32
    MSG_RemoveMuteListEntry              = MK_LOW(264), // AgentID: LLUUID, SessionID: LLUUID, MuteID: LLUUID
    MSG_CopyInventoryFromNotecard        = MK_LOW(265), // AgentID: LLUUID, SessionID: LLUUID, NotecardItemID: LLUUID, ObjectID: LLUUID, ItemID: LLUUID, FolderID: LLUUID
    MSG_UpdateInventoryItem              = MK_LOW(266), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, ItemID: LLUUID, FolderID: LLUUID, CallbackID: U32, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, TransactionID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_UpdateCreateInventoryItem        = MK_LOW(267), // AgentID: LLUUID, SimApproved: BOOL, TransactionID: LLUUID, ItemID: LLUUID, FolderID: LLUUID, CallbackID: U32, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, AssetID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_MoveInventoryItem                = MK_LOW(268), // AgentID: LLUUID, SessionID: LLUUID, Stamp: BOOL, ItemID: LLUUID, FolderID: LLUUID
    MSG_CopyInventoryItem                = MK_LOW(269), // AgentID: LLUUID, SessionID: LLUUID, CallbackID: U32, OldAgentID: LLUUID, OldItemID: LLUUID, NewFolderID: LLUUID
    MSG_RemoveInventoryItem              = MK_LOW(270), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID
    MSG_ChangeInventoryItemFlags         = MK_LOW(271), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID, Flags: U32
    MSG_SaveAssetIntoInventory           = MK_LOW(272), // AgentID: LLUUID, ItemID: LLUUID, NewAssetID: LLUUID
    MSG_CreateInventoryFolder            = MK_LOW(273), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID, ParentID: LLUUID, Type: S8
    MSG_UpdateInventoryFolder            = MK_LOW(274), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID, ParentID: LLUUID, Type: S8
    MSG_MoveInventoryFolder              = MK_LOW(275), // AgentID: LLUUID, SessionID: LLUUID, Stamp: BOOL, FolderID: LLUUID, ParentID: LLUUID
    MSG_RemoveInventoryFolder            = MK_LOW(276), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID
    MSG_FetchInventoryDescendents        = MK_LOW(277), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID, OwnerID: LLUUID, SortOrder: S32, FetchFolders: BOOL, FetchItems: BOOL
    MSG_InventoryDescendents             = MK_LOW(278), // AgentID: LLUUID, FolderID: LLUUID, OwnerID: LLUUID, Version: S32, Descendents: S32, FolderID: LLUUID, ParentID: LLUUID, Type: S8, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, AssetID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_FetchInventory                   = MK_LOW(279), // AgentID: LLUUID, SessionID: LLUUID, OwnerID: LLUUID, ItemID: LLUUID
    MSG_FetchInventoryReply              = MK_LOW(280), // AgentID: LLUUID, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, AssetID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_BulkUpdateInventory              = MK_LOW(281), // AgentID: LLUUID, TransactionID: LLUUID, FolderID: LLUUID, ParentID: LLUUID, Type: S8, ItemID: LLUUID, CallbackID: U32, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, AssetID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_RequestInventoryAsset            = MK_LOW(282), // QueryID: LLUUID, AgentID: LLUUID, OwnerID: LLUUID, ItemID: LLUUID
    MSG_InventoryAssetResponse           = MK_LOW(283), // QueryID: LLUUID, AssetID: LLUUID, IsReadable: BOOL
    MSG_RemoveInventoryObjects           = MK_LOW(284), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID, ItemID: LLUUID
    MSG_PurgeInventoryDescendents        = MK_LOW(285), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID
    MSG_UpdateTaskInventory              = MK_LOW(286), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, Key: U8, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, TransactionID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_RemoveTaskInventory              = MK_LOW(287), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32, ItemID: LLUUID
    MSG_MoveTaskInventory                = MK_LOW(288), // AgentID: LLUUID, SessionID: LLUUID, FolderID: LLUUID, LocalID: U32, ItemID: LLUUID
    MSG_RequestTaskInventory             = MK_LOW(289), // AgentID: LLUUID, SessionID: LLUUID, LocalID: U32
    MSG_ReplyTaskInventory               = MK_LOW(290), // TaskID: LLUUID, Serial: S16
    MSG_DeRezObject                      = MK_LOW(291), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, Destination: U8, DestinationID: LLUUID, TransactionID: LLUUID, PacketCount: U8, PacketNumber: U8, ObjectLocalID: U32
    MSG_DeRezAck                         = MK_LOW(292), // TransactionID: LLUUID, Success: BOOL
    MSG_RezObject                        = MK_LOW(293), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, FromTaskID: LLUUID, BypassRaycast: U8, RayStart: LLVector3, RayEnd: LLVector3, RayTargetID: LLUUID, RayEndIsIntersection: BOOL, RezSelected: BOOL, RemoveItem: BOOL, ItemFlags: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, TransactionID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_RezObjectFromNotecard            = MK_LOW(294), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, FromTaskID: LLUUID, BypassRaycast: U8, RayStart: LLVector3, RayEnd: LLVector3, RayTargetID: LLUUID, RayEndIsIntersection: BOOL, RezSelected: BOOL, RemoveItem: BOOL, ItemFlags: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, NotecardItemID: LLUUID, ObjectID: LLUUID, ItemID: LLUUID
    MSG_TransferInventory                = MK_LOW(295), // SourceID: LLUUID, DestID: LLUUID, TransactionID: LLUUID, InventoryID: LLUUID, Type: S8, NeedsValidation: BOOL, EstateID: U32
    MSG_TransferInventoryAck             = MK_LOW(296), // TransactionID: LLUUID, InventoryID: LLUUID
    MSG_AcceptFriendship                 = MK_LOW(297), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, FolderID: LLUUID
    MSG_DeclineFriendship                = MK_LOW(298), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID
    MSG_FormFriendship                   = MK_LOW(299), // SourceID: LLUUID, DestID: LLUUID
    MSG_TerminateFriendship              = MK_LOW(300), // AgentID: LLUUID, SessionID: LLUUID, OtherID: LLUUID
    MSG_OfferCallingCard                 = MK_LOW(301), // AgentID: LLUUID, SessionID: LLUUID, DestID: LLUUID, TransactionID: LLUUID
    MSG_AcceptCallingCard                = MK_LOW(302), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID, FolderID: LLUUID
    MSG_DeclineCallingCard               = MK_LOW(303), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID
    MSG_RezScript                        = MK_LOW(304), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, ObjectLocalID: U32, Enabled: BOOL, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, TransactionID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32, TemplateID: LLUUID
    MSG_CreateInventoryItem              = MK_LOW(305), // AgentID: LLUUID, SessionID: LLUUID, CallbackID: U32, FolderID: LLUUID, TransactionID: LLUUID, NextOwnerMask: U32, Type: S8, InvType: S8, WearableType: U8
    MSG_CreateLandmarkForEvent           = MK_LOW(306), // AgentID: LLUUID, SessionID: LLUUID, EventID: U32, FolderID: LLUUID
    MSG_EventLocationRequest             = MK_LOW(307), // QueryID: LLUUID, EventID: U32
    MSG_EventLocationReply               = MK_LOW(308), // QueryID: LLUUID, Success: BOOL, RegionID: LLUUID, RegionPos: LLVector3
    MSG_RegionHandleRequest              = MK_LOW(309), // RegionID: LLUUID
    MSG_RegionIDAndHandleReply           = MK_LOW(310), // RegionID: LLUUID, RegionHandle: U64
    MSG_MoneyTransferRequest             = MK_LOW(311), // AgentID: LLUUID, SessionID: LLUUID, SourceID: LLUUID, DestID: LLUUID, Flags: U8, Amount: S32, AggregatePermNextOwner: U8, AggregatePermInventory: U8, TransactionType: S32
    MSG_MoneyTransferBackend             = MK_LOW(312), // TransactionID: LLUUID, TransactionTime: U32, SourceID: LLUUID, DestID: LLUUID, Flags: U8, Amount: S32, AggregatePermNextOwner: U8, AggregatePermInventory: U8, TransactionType: S32, RegionID: LLUUID, GridX: U32, GridY: U32
    MSG_MoneyBalanceRequest              = MK_LOW(313), // AgentID: LLUUID, SessionID: LLUUID, TransactionID: LLUUID
    MSG_MoneyBalanceReply                = MK_LOW(314), // AgentID: LLUUID, TransactionID: LLUUID, TransactionSuccess: BOOL, MoneyBalance: S32, SquareMetersCredit: S32, SquareMetersCommitted: S32, TransactionType: S32, SourceID: LLUUID, IsSourceGroup: BOOL, DestID: LLUUID, IsDestGroup: BOOL, Amount: S32
    MSG_RoutedMoneyBalanceReply          = MK_LOW(315), // TargetIP: IPADDR, TargetPort: IPPORT, AgentID: LLUUID, TransactionID: LLUUID, TransactionSuccess: BOOL, MoneyBalance: S32, SquareMetersCredit: S32, SquareMetersCommitted: S32, TransactionType: S32, SourceID: LLUUID, IsSourceGroup: BOOL, DestID: LLUUID, IsDestGroup: BOOL, Amount: S32
    MSG_ActivateGestures                 = MK_LOW(316), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, ItemID: LLUUID, AssetID: LLUUID, GestureFlags: U32
    MSG_DeactivateGestures               = MK_LOW(317), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, ItemID: LLUUID, GestureFlags: U32
    MSG_MuteListUpdate                   = MK_LOW(318), // AgentID: LLUUID
    MSG_UseCachedMuteList                = MK_LOW(319), // AgentID: LLUUID
    MSG_GrantUserRights                  = MK_LOW(320), // AgentID: LLUUID, SessionID: LLUUID, AgentRelated: LLUUID, RelatedRights: S32
    MSG_ChangeUserRights                 = MK_LOW(321), // AgentID: LLUUID, AgentRelated: LLUUID, RelatedRights: S32
    MSG_OnlineNotification               = MK_LOW(322), // AgentID: LLUUID
    MSG_OfflineNotification              = MK_LOW(323), // AgentID: LLUUID
    MSG_SetStartLocationRequest          = MK_LOW(324), // AgentID: LLUUID, SessionID: LLUUID, LocationID: U32, LocationPos: LLVector3, LocationLookAt: LLVector3
    MSG_SetStartLocation                 = MK_LOW(325), // AgentID: LLUUID, RegionID: LLUUID, LocationID: U32, RegionHandle: U64, LocationPos: LLVector3, LocationLookAt: LLVector3
    MSG_NetTest                          = MK_LOW(326), // Port: IPPORT
    MSG_SetCPURatio                      = MK_LOW(327), // Ratio: U8
    MSG_SimCrashed                       = MK_LOW(328), // RegionX: U32, RegionY: U32, AgentID: LLUUID
    MSG_NameValuePair                    = MK_LOW(329), // ID: LLUUID
    MSG_RemoveNameValuePair              = MK_LOW(330), // ID: LLUUID
    MSG_UpdateAttachment                 = MK_LOW(331), // AgentID: LLUUID, SessionID: LLUUID, AttachmentPoint: U8, AddItem: BOOL, UseExistingAsset: BOOL, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, AssetID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_RemoveAttachment                 = MK_LOW(332), // AgentID: LLUUID, SessionID: LLUUID, AttachmentPoint: U8, ItemID: LLUUID
    MSG_SoundTrigger                     = MK_HIGH(29), // SoundID: LLUUID, OwnerID: LLUUID, ObjectID: LLUUID, ParentID: LLUUID, Handle: U64, Position: LLVector3, Gain: F32
    MSG_AttachedSound                    = MK_MED(13), // SoundID: LLUUID, ObjectID: LLUUID, OwnerID: LLUUID, Gain: F32, Flags: U8
    MSG_AttachedSoundGainChange          = MK_MED(14), // ObjectID: LLUUID, Gain: F32
    MSG_PreloadSound                     = MK_MED(15), // ObjectID: LLUUID, OwnerID: LLUUID, SoundID: LLUUID
    MSG_ObjectAnimation                  = MK_HIGH(30), // ID: LLUUID, AnimID: LLUUID, AnimSequenceID: S32
    MSG_AssetUploadRequest               = MK_LOW(333), // TransactionID: LLUUID, Type: S8, Tempfile: BOOL, StoreLocal: BOOL
    MSG_AssetUploadComplete              = MK_LOW(334), // UUID: LLUUID, Type: S8, Success: BOOL
    MSG_EmailMessageRequest              = MK_LOW(335), // ObjectID: LLUUID
    MSG_EmailMessageReply                = MK_LOW(336), // ObjectID: LLUUID, More: U32, Time: U32
    MSG_InternalScriptMail               = MK_MED(16), // To: LLUUID
    MSG_ScriptDataRequest                = MK_LOW(337), // Hash: U64, RequestType: S8
    MSG_ScriptDataReply                  = MK_LOW(338), // Hash: U64
    MSG_CreateGroupRequest               = MK_LOW(339), // AgentID: LLUUID, SessionID: LLUUID, ShowInList: BOOL, InsigniaID: LLUUID, MembershipFee: S32, OpenEnrollment: BOOL, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_CreateGroupReply                 = MK_LOW(340), // AgentID: LLUUID, GroupID: LLUUID, Success: BOOL
    MSG_UpdateGroupInfo                  = MK_LOW(341), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, ShowInList: BOOL, InsigniaID: LLUUID, MembershipFee: S32, OpenEnrollment: BOOL, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_GroupRoleChanges                 = MK_LOW(342), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RoleID: LLUUID, MemberID: LLUUID, Change: U32
    MSG_JoinGroupRequest                 = MK_LOW(343), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID
    MSG_JoinGroupReply                   = MK_LOW(344), // AgentID: LLUUID, GroupID: LLUUID, Success: BOOL
    MSG_EjectGroupMemberRequest          = MK_LOW(345), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, EjecteeID: LLUUID
    MSG_EjectGroupMemberReply            = MK_LOW(346), // AgentID: LLUUID, GroupID: LLUUID, Success: BOOL
    MSG_LeaveGroupRequest                = MK_LOW(347), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID
    MSG_LeaveGroupReply                  = MK_LOW(348), // AgentID: LLUUID, GroupID: LLUUID, Success: BOOL
    MSG_InviteGroupRequest               = MK_LOW(349), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, InviteeID: LLUUID, RoleID: LLUUID
    MSG_InviteGroupResponse              = MK_LOW(350), // AgentID: LLUUID, InviteeID: LLUUID, GroupID: LLUUID, RoleID: LLUUID, MembershipFee: S32, GroupLimit: S32
    MSG_GroupProfileRequest              = MK_LOW(351), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID
    MSG_GroupProfileReply                = MK_LOW(352), // AgentID: LLUUID, GroupID: LLUUID, ShowInList: BOOL, PowersMask: U64, InsigniaID: LLUUID, FounderID: LLUUID, MembershipFee: S32, OpenEnrollment: BOOL, Money: S32, GroupMembershipCount: S32, GroupRolesCount: S32, AllowPublish: BOOL, MaturePublish: BOOL, OwnerRole: LLUUID
    MSG_GroupAccountSummaryRequest       = MK_LOW(353), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32
    MSG_GroupAccountSummaryReply         = MK_LOW(354), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32, Balance: S32, TotalCredits: S32, TotalDebits: S32, ObjectTaxCurrent: S32, LightTaxCurrent: S32, LandTaxCurrent: S32, GroupTaxCurrent: S32, ParcelDirFeeCurrent: S32, ObjectTaxEstimate: S32, LightTaxEstimate: S32, LandTaxEstimate: S32, GroupTaxEstimate: S32, ParcelDirFeeEstimate: S32, NonExemptMembers: S32
    MSG_GroupAccountDetailsRequest       = MK_LOW(355), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32
    MSG_GroupAccountDetailsReply         = MK_LOW(356), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32, Amount: S32
    MSG_GroupAccountTransactionsRequest  = MK_LOW(357), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32
    MSG_GroupAccountTransactionsReply    = MK_LOW(358), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, IntervalDays: S32, CurrentInterval: S32, Type: S32, Amount: S32
    MSG_GroupActiveProposalsRequest      = MK_LOW(359), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, TransactionID: LLUUID
    MSG_GroupActiveProposalItemReply     = MK_LOW(360), // AgentID: LLUUID, GroupID: LLUUID, TransactionID: LLUUID, TotalNumItems: U32, VoteID: LLUUID, VoteInitiator: LLUUID, AlreadyVoted: BOOL, Majority: F32, Quorum: S32
    MSG_GroupVoteHistoryRequest          = MK_LOW(361), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, TransactionID: LLUUID
    MSG_GroupVoteHistoryItemReply        = MK_LOW(362), // AgentID: LLUUID, GroupID: LLUUID, TransactionID: LLUUID, TotalNumItems: U32, VoteID: LLUUID, VoteInitiator: LLUUID, Majority: F32, Quorum: S32, CandidateID: LLUUID, NumVotes: S32
    MSG_StartGroupProposal               = MK_LOW(363), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, Quorum: S32, Majority: F32, Duration: S32
    MSG_GroupProposalBallot              = MK_LOW(364), // AgentID: LLUUID, SessionID: LLUUID, ProposalID: LLUUID, GroupID: LLUUID
    MSG_TallyVotes                       = MK_LOW(365),
    MSG_GroupMembersRequest              = MK_LOW(366), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID
    MSG_GroupMembersReply                = MK_LOW(367), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, MemberCount: S32, AgentID: LLUUID, Contribution: S32, AgentPowers: U64, IsOwner: BOOL
    MSG_ActivateGroup                    = MK_LOW(368), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID
    MSG_SetGroupContribution             = MK_LOW(369), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, Contribution: S32
    MSG_SetGroupAcceptNotices            = MK_LOW(370), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, AcceptNotices: BOOL, ListInProfile: BOOL
    MSG_GroupRoleDataRequest             = MK_LOW(371), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID
    MSG_GroupRoleDataReply               = MK_LOW(372), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, RoleCount: S32, RoleID: LLUUID, Powers: U64, Members: U32
    MSG_GroupRoleMembersRequest          = MK_LOW(373), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID
    MSG_GroupRoleMembersReply            = MK_LOW(374), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, TotalPairs: U32, RoleID: LLUUID, MemberID: LLUUID
    MSG_GroupTitlesRequest               = MK_LOW(375), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RequestID: LLUUID
    MSG_GroupTitlesReply                 = MK_LOW(376), // AgentID: LLUUID, GroupID: LLUUID, RequestID: LLUUID, RoleID: LLUUID, Selected: BOOL
    MSG_GroupTitleUpdate                 = MK_LOW(377), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, TitleRoleID: LLUUID
    MSG_GroupRoleUpdate                  = MK_LOW(378), // AgentID: LLUUID, SessionID: LLUUID, GroupID: LLUUID, RoleID: LLUUID, Powers: U64, UpdateType: U8
    MSG_LiveHelpGroupRequest             = MK_LOW(379), // RequestID: LLUUID, AgentID: LLUUID
    MSG_LiveHelpGroupReply               = MK_LOW(380), // RequestID: LLUUID, GroupID: LLUUID
    MSG_AgentWearablesRequest            = MK_LOW(381), // AgentID: LLUUID, SessionID: LLUUID
    MSG_AgentWearablesUpdate             = MK_LOW(382), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: U32, ItemID: LLUUID, AssetID: LLUUID, WearableType: U8
    MSG_AgentIsNowWearing                = MK_LOW(383), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID, WearableType: U8
    MSG_AgentCachedTexture               = MK_LOW(384), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: S32, ID: LLUUID, TextureIndex: U8
    MSG_AgentCachedTextureResponse       = MK_LOW(385), // AgentID: LLUUID, SessionID: LLUUID, SerialNum: S32, TextureID: LLUUID, TextureIndex: U8
    MSG_AgentDataUpdateRequest           = MK_LOW(386), // AgentID: LLUUID, SessionID: LLUUID
    MSG_AgentDataUpdate                  = MK_LOW(387), // AgentID: LLUUID, ActiveGroupID: LLUUID, GroupPowers: U64
    MSG_GroupDataUpdate                  = MK_LOW(388), // AgentID: LLUUID, GroupID: LLUUID, AgentPowers: U64
    MSG_AgentGroupDataUpdate             = MK_LOW(389), // AgentID: LLUUID, GroupID: LLUUID, GroupPowers: U64, AcceptNotices: BOOL, GroupInsigniaID: LLUUID, Contribution: S32
    MSG_AgentDropGroup                   = MK_LOW(390), // AgentID: LLUUID, GroupID: LLUUID
    MSG_LogTextMessage                   = MK_LOW(391), // FromAgentId: LLUUID, ToAgentId: LLUUID, GlobalX: F64, GlobalY: F64, Time: U32
    MSG_ViewerEffect                     = MK_MED(17), // AgentID: LLUUID, SessionID: LLUUID, ID: LLUUID, AgentID: LLUUID, Type: U8, Duration: F32
    MSG_CreateTrustedCircuit             = MK_LOW(392), // EndPointID: LLUUID
    MSG_DenyTrustedCircuit               = MK_LOW(393), // EndPointID: LLUUID
    MSG_RequestTrustedCircuit            = MK_LOW(394),
    MSG_RezSingleAttachmentFromInv       = MK_LOW(395), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID, OwnerID: LLUUID, AttachmentPt: U8, ItemFlags: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32
    MSG_RezMultipleAttachmentsFromInv    = MK_LOW(396), // AgentID: LLUUID, SessionID: LLUUID, CompoundMsgID: LLUUID, TotalObjects: U8, FirstDetachAll: BOOL, ItemID: LLUUID, OwnerID: LLUUID, AttachmentPt: U8, ItemFlags: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32
    MSG_DetachAttachmentIntoInv          = MK_LOW(397), // AgentID: LLUUID, ItemID: LLUUID
    MSG_CreateNewOutfitAttachments       = MK_LOW(398), // AgentID: LLUUID, SessionID: LLUUID, NewFolderID: LLUUID, OldItemID: LLUUID, OldFolderID: LLUUID
    MSG_UserInfoRequest                  = MK_LOW(399), // AgentID: LLUUID, SessionID: LLUUID
    MSG_UserInfoReply                    = MK_LOW(400), // AgentID: LLUUID, IMViaEMail: BOOL
    MSG_UpdateUserInfo                   = MK_LOW(401), // AgentID: LLUUID, SessionID: LLUUID, IMViaEMail: BOOL
    MSG_ParcelRename                     = MK_LOW(402), // ParcelID: LLUUID
    MSG_InitiateDownload                 = MK_LOW(403), // AgentID: LLUUID
    MSG_SystemMessage                    = MK_LOW(404), // Invoice: LLUUID
    MSG_MapLayerRequest                  = MK_LOW(405), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, EstateID: U32, Godlike: BOOL
    MSG_MapLayerReply                    = MK_LOW(406), // AgentID: LLUUID, Flags: U32, Left: U32, Right: U32, Top: U32, Bottom: U32, ImageID: LLUUID
    MSG_MapBlockRequest                  = MK_LOW(407), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, EstateID: U32, Godlike: BOOL, MinX: U16, MaxX: U16, MinY: U16, MaxY: U16
    MSG_MapNameRequest                   = MK_LOW(408), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, EstateID: U32, Godlike: BOOL
    MSG_MapBlockReply                    = MK_LOW(409), // AgentID: LLUUID, Flags: U32, X: U16, Y: U16, Access: U8, RegionFlags: U32, WaterHeight: U8, Agents: U8, MapImageID: LLUUID
    MSG_MapItemRequest                   = MK_LOW(410), // AgentID: LLUUID, SessionID: LLUUID, Flags: U32, EstateID: U32, Godlike: BOOL, ItemType: U32, RegionHandle: U64
    MSG_MapItemReply                     = MK_LOW(411), // AgentID: LLUUID, Flags: U32, ItemType: U32, X: U32, Y: U32, ID: LLUUID, Extra: S32, Extra2: S32
    MSG_SendPostcard                     = MK_LOW(412), // AgentID: LLUUID, SessionID: LLUUID, AssetID: LLUUID, PosGlobal: LLVector3d, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_RpcChannelRequest                = MK_LOW(413), // GridX: U32, GridY: U32, TaskID: LLUUID, ItemID: LLUUID
    MSG_RpcChannelReply                  = MK_LOW(414), // TaskID: LLUUID, ItemID: LLUUID, ChannelID: LLUUID
    MSG_RpcScriptRequestInbound          = MK_LOW(415), // GridX: U32, GridY: U32, TaskID: LLUUID, ItemID: LLUUID, ChannelID: LLUUID, IntValue: U32
    MSG_RpcScriptRequestInboundForward   = MK_LOW(416), // RPCServerIP: IPADDR, RPCServerPort: IPPORT, TaskID: LLUUID, ItemID: LLUUID, ChannelID: LLUUID, IntValue: U32
    MSG_RpcScriptReplyInbound            = MK_LOW(417), // TaskID: LLUUID, ItemID: LLUUID, ChannelID: LLUUID, IntValue: U32
    MSG_ScriptMailRegistration           = MK_LOW(418), // TargetPort: IPPORT, TaskID: LLUUID, Flags: U32
    MSG_ParcelMediaCommandMessage        = MK_LOW(419), // Flags: U32, Command: U32, Time: F32
    MSG_ParcelMediaUpdate                = MK_LOW(420), // MediaID: LLUUID, MediaAutoScale: U8, MediaWidth: S32, MediaHeight: S32, MediaLoop: U8
    MSG_LandStatRequest                  = MK_LOW(421), // AgentID: LLUUID, SessionID: LLUUID, ReportType: U32, RequestFlags: U32, ParcelLocalID: S32
    MSG_LandStatReply                    = MK_LOW(422), // ReportType: U32, RequestFlags: U32, TotalObjectCount: U32, TaskLocalID: U32, TaskID: LLUUID, LocationX: F32, LocationY: F32, LocationZ: F32, Score: F32
    MSG_Error                            = MK_LOW(423), // AgentID: LLUUID, Code: S32, ID: LLUUID
    MSG_ObjectIncludeInSearch            = MK_LOW(424), // AgentID: LLUUID, SessionID: LLUUID, ObjectLocalID: U32, IncludeInSearch: BOOL
    MSG_RezRestoreToWorld                = MK_LOW(425), // AgentID: LLUUID, SessionID: LLUUID, ItemID: LLUUID, FolderID: LLUUID, CreatorID: LLUUID, OwnerID: LLUUID, GroupID: LLUUID, BaseMask: U32, OwnerMask: U32, GroupMask: U32, EveryoneMask: U32, NextOwnerMask: U32, GroupOwned: BOOL, TransactionID: LLUUID, Type: S8, InvType: S8, Flags: U32, SaleType: U8, SalePrice: S32, CreationDate: S32, CRC: U32
    MSG_LinkInventoryItem                = MK_LOW(426), // AgentID: LLUUID, SessionID: LLUUID, CallbackID: U32, FolderID: LLUUID, TransactionID: LLUUID, OldItemID: LLUUID, Type: S8, InvType: S8
    MSG_RetrieveIMsExtended              = MK_LOW(427), // AgentID: LLUUID, SessionID: LLUUID, IsPremium: BOOL
    MSG_JoinGroupRequestExtended         = MK_LOW(428), // AgentID: LLUUID, SessionID: LLUUID, GroupLimit: S32, GroupID: LLUUID
    MSG_CreateGroupRequestExtended       = MK_LOW(429), // AgentID: LLUUID, SessionID: LLUUID, GroupLimit: S32, ShowInList: BOOL, InsigniaID: LLUUID, MembershipFee: S32, OpenEnrollment: BOOL, AllowPublish: BOOL, MaturePublish: BOOL
    MSG_GameControlInput                 = MK_HIGH(32), // AgentID: LLUUID, SessionID: LLUUID, Index: U8, Value: S16
};

static inline const char *message_name(uint32_t id)
{
    switch (id) {

    case MSG_TestMessage: return "TestMessage";
    case MSG_PacketAck: return "PacketAck";
    case MSG_OpenCircuit: return "OpenCircuit";
    case MSG_CloseCircuit: return "CloseCircuit";
    case MSG_StartPingCheck: return "StartPingCheck";
    case MSG_CompletePingCheck: return "CompletePingCheck";
    case MSG_AddCircuitCode: return "AddCircuitCode";
    case MSG_UseCircuitCode: return "UseCircuitCode";
    case MSG_NeighborList: return "NeighborList";
    case MSG_AvatarTextureUpdate: return "AvatarTextureUpdate";
    case MSG_SimulatorMapUpdate: return "SimulatorMapUpdate";
    case MSG_SimulatorSetMap: return "SimulatorSetMap";
    case MSG_SubscribeLoad: return "SubscribeLoad";
    case MSG_UnsubscribeLoad: return "UnsubscribeLoad";
    case MSG_SimulatorReady: return "SimulatorReady";
    case MSG_TelehubInfo: return "TelehubInfo";
    case MSG_SimulatorPresentAtLocation: return "SimulatorPresentAtLocation";
    case MSG_SimulatorLoad: return "SimulatorLoad";
    case MSG_SimulatorShutdownRequest: return "SimulatorShutdownRequest";
    case MSG_RegionPresenceRequestByRegionID: return "RegionPresenceRequestByRegionID";
    case MSG_RegionPresenceRequestByHandle: return "RegionPresenceRequestByHandle";
    case MSG_RegionPresenceResponse: return "RegionPresenceResponse";
    case MSG_UpdateSimulator: return "UpdateSimulator";
    case MSG_LogDwellTime: return "LogDwellTime";
    case MSG_FeatureDisabled: return "FeatureDisabled";
    case MSG_LogFailedMoneyTransaction: return "LogFailedMoneyTransaction";
    case MSG_UserReportInternal: return "UserReportInternal";
    case MSG_SetSimStatusInDatabase: return "SetSimStatusInDatabase";
    case MSG_SetSimPresenceInDatabase: return "SetSimPresenceInDatabase";
    case MSG_EconomyDataRequest: return "EconomyDataRequest";
    case MSG_EconomyData: return "EconomyData";
    case MSG_AvatarPickerRequest: return "AvatarPickerRequest";
    case MSG_AvatarPickerRequestBackend: return "AvatarPickerRequestBackend";
    case MSG_AvatarPickerReply: return "AvatarPickerReply";
    case MSG_PlacesQuery: return "PlacesQuery";
    case MSG_PlacesReply: return "PlacesReply";
    case MSG_DirFindQuery: return "DirFindQuery";
    case MSG_DirFindQueryBackend: return "DirFindQueryBackend";
    case MSG_DirPlacesQuery: return "DirPlacesQuery";
    case MSG_DirPlacesQueryBackend: return "DirPlacesQueryBackend";
    case MSG_DirPlacesReply: return "DirPlacesReply";
    case MSG_DirPeopleReply: return "DirPeopleReply";
    case MSG_DirEventsReply: return "DirEventsReply";
    case MSG_DirGroupsReply: return "DirGroupsReply";
    case MSG_DirClassifiedQuery: return "DirClassifiedQuery";
    case MSG_DirClassifiedQueryBackend: return "DirClassifiedQueryBackend";
    case MSG_DirClassifiedReply: return "DirClassifiedReply";
    case MSG_AvatarClassifiedReply: return "AvatarClassifiedReply";
    case MSG_ClassifiedInfoRequest: return "ClassifiedInfoRequest";
    case MSG_ClassifiedInfoReply: return "ClassifiedInfoReply";
    case MSG_ClassifiedInfoUpdate: return "ClassifiedInfoUpdate";
    case MSG_ClassifiedDelete: return "ClassifiedDelete";
    case MSG_ClassifiedGodDelete: return "ClassifiedGodDelete";
    case MSG_DirLandQuery: return "DirLandQuery";
    case MSG_DirLandQueryBackend: return "DirLandQueryBackend";
    case MSG_DirLandReply: return "DirLandReply";
    case MSG_DirPopularQuery: return "DirPopularQuery";
    case MSG_DirPopularQueryBackend: return "DirPopularQueryBackend";
    case MSG_DirPopularReply: return "DirPopularReply";
    case MSG_ParcelInfoRequest: return "ParcelInfoRequest";
    case MSG_ParcelInfoReply: return "ParcelInfoReply";
    case MSG_ParcelObjectOwnersRequest: return "ParcelObjectOwnersRequest";
    case MSG_ParcelObjectOwnersReply: return "ParcelObjectOwnersReply";
    case MSG_GroupNoticesListRequest: return "GroupNoticesListRequest";
    case MSG_GroupNoticesListReply: return "GroupNoticesListReply";
    case MSG_GroupNoticeRequest: return "GroupNoticeRequest";
    case MSG_GroupNoticeAdd: return "GroupNoticeAdd";
    case MSG_TeleportRequest: return "TeleportRequest";
    case MSG_TeleportLocationRequest: return "TeleportLocationRequest";
    case MSG_TeleportLocal: return "TeleportLocal";
    case MSG_TeleportLandmarkRequest: return "TeleportLandmarkRequest";
    case MSG_TeleportProgress: return "TeleportProgress";
    case MSG_DataHomeLocationRequest: return "DataHomeLocationRequest";
    case MSG_DataHomeLocationReply: return "DataHomeLocationReply";
    case MSG_TeleportFinish: return "TeleportFinish";
    case MSG_StartLure: return "StartLure";
    case MSG_TeleportLureRequest: return "TeleportLureRequest";
    case MSG_TeleportCancel: return "TeleportCancel";
    case MSG_TeleportStart: return "TeleportStart";
    case MSG_TeleportFailed: return "TeleportFailed";
    case MSG_Undo: return "Undo";
    case MSG_Redo: return "Redo";
    case MSG_UndoLand: return "UndoLand";
    case MSG_AgentPause: return "AgentPause";
    case MSG_AgentResume: return "AgentResume";
    case MSG_AgentUpdate: return "AgentUpdate";
    case MSG_ChatFromViewer: return "ChatFromViewer";
    case MSG_AgentThrottle: return "AgentThrottle";
    case MSG_AgentFOV: return "AgentFOV";
    case MSG_AgentHeightWidth: return "AgentHeightWidth";
    case MSG_AgentSetAppearance: return "AgentSetAppearance";
    case MSG_AgentAnimation: return "AgentAnimation";
    case MSG_AgentRequestSit: return "AgentRequestSit";
    case MSG_AgentSit: return "AgentSit";
    case MSG_AgentQuitCopy: return "AgentQuitCopy";
    case MSG_RequestImage: return "RequestImage";
    case MSG_ImageNotInDatabase: return "ImageNotInDatabase";
    case MSG_RebakeAvatarTextures: return "RebakeAvatarTextures";
    case MSG_SetAlwaysRun: return "SetAlwaysRun";
    case MSG_ObjectAdd: return "ObjectAdd";
    case MSG_ObjectDelete: return "ObjectDelete";
    case MSG_ObjectDuplicate: return "ObjectDuplicate";
    case MSG_ObjectDuplicateOnRay: return "ObjectDuplicateOnRay";
    case MSG_MultipleObjectUpdate: return "MultipleObjectUpdate";
    case MSG_RequestMultipleObjects: return "RequestMultipleObjects";
    case MSG_ObjectPosition: return "ObjectPosition";
    case MSG_ObjectScale: return "ObjectScale";
    case MSG_ObjectRotation: return "ObjectRotation";
    case MSG_ObjectFlagUpdate: return "ObjectFlagUpdate";
    case MSG_ObjectClickAction: return "ObjectClickAction";
    case MSG_ObjectImage: return "ObjectImage";
    case MSG_ObjectBypassModUpdate: return "ObjectBypassModUpdate";
    case MSG_ObjectMaterial: return "ObjectMaterial";
    case MSG_ObjectShape: return "ObjectShape";
    case MSG_ObjectExtraParams: return "ObjectExtraParams";
    case MSG_ObjectOwner: return "ObjectOwner";
    case MSG_ObjectGroup: return "ObjectGroup";
    case MSG_ObjectBuy: return "ObjectBuy";
    case MSG_BuyObjectInventory: return "BuyObjectInventory";
    case MSG_DerezContainer: return "DerezContainer";
    case MSG_ObjectPermissions: return "ObjectPermissions";
    case MSG_ObjectSaleInfo: return "ObjectSaleInfo";
    case MSG_ObjectName: return "ObjectName";
    case MSG_ObjectDescription: return "ObjectDescription";
    case MSG_ObjectCategory: return "ObjectCategory";
    case MSG_ObjectSelect: return "ObjectSelect";
    case MSG_ObjectDeselect: return "ObjectDeselect";
    case MSG_ObjectAttach: return "ObjectAttach";
    case MSG_ObjectDetach: return "ObjectDetach";
    case MSG_ObjectDrop: return "ObjectDrop";
    case MSG_ObjectLink: return "ObjectLink";
    case MSG_ObjectDelink: return "ObjectDelink";
    case MSG_ObjectGrab: return "ObjectGrab";
    case MSG_ObjectGrabUpdate: return "ObjectGrabUpdate";
    case MSG_ObjectDeGrab: return "ObjectDeGrab";
    case MSG_ObjectSpinStart: return "ObjectSpinStart";
    case MSG_ObjectSpinUpdate: return "ObjectSpinUpdate";
    case MSG_ObjectSpinStop: return "ObjectSpinStop";
    case MSG_ObjectExportSelected: return "ObjectExportSelected";
    case MSG_ModifyLand: return "ModifyLand";
    case MSG_VelocityInterpolateOn: return "VelocityInterpolateOn";
    case MSG_VelocityInterpolateOff: return "VelocityInterpolateOff";
    case MSG_StateSave: return "StateSave";
    case MSG_ReportAutosaveCrash: return "ReportAutosaveCrash";
    case MSG_SimWideDeletes: return "SimWideDeletes";
    case MSG_RequestObjectPropertiesFamily: return "RequestObjectPropertiesFamily";
    case MSG_TrackAgent: return "TrackAgent";
    case MSG_ViewerStats: return "ViewerStats";
    case MSG_ScriptAnswerYes: return "ScriptAnswerYes";
    case MSG_UserReport: return "UserReport";
    case MSG_AlertMessage: return "AlertMessage";
    case MSG_AgentAlertMessage: return "AgentAlertMessage";
    case MSG_MeanCollisionAlert: return "MeanCollisionAlert";
    case MSG_ViewerFrozenMessage: return "ViewerFrozenMessage";
    case MSG_HealthMessage: return "HealthMessage";
    case MSG_ChatFromSimulator: return "ChatFromSimulator";
    case MSG_SimStats: return "SimStats";
    case MSG_RequestRegionInfo: return "RequestRegionInfo";
    case MSG_RegionInfo: return "RegionInfo";
    case MSG_GodUpdateRegionInfo: return "GodUpdateRegionInfo";
    case MSG_NearestLandingRegionRequest: return "NearestLandingRegionRequest";
    case MSG_NearestLandingRegionReply: return "NearestLandingRegionReply";
    case MSG_NearestLandingRegionUpdated: return "NearestLandingRegionUpdated";
    case MSG_TeleportLandingStatusChanged: return "TeleportLandingStatusChanged";
    case MSG_RegionHandshake: return "RegionHandshake";
    case MSG_RegionHandshakeReply: return "RegionHandshakeReply";
    case MSG_CoarseLocationUpdate: return "CoarseLocationUpdate";
    case MSG_ImageData: return "ImageData";
    case MSG_ImagePacket: return "ImagePacket";
    case MSG_LayerData: return "LayerData";
    case MSG_ObjectUpdate: return "ObjectUpdate";
    case MSG_ObjectUpdateCompressed: return "ObjectUpdateCompressed";
    case MSG_ObjectUpdateCached: return "ObjectUpdateCached";
    case MSG_ImprovedTerseObjectUpdate: return "ImprovedTerseObjectUpdate";
    case MSG_KillObject: return "KillObject";
    case MSG_CrossedRegion: return "CrossedRegion";
    case MSG_SimulatorViewerTimeMessage: return "SimulatorViewerTimeMessage";
    case MSG_EnableSimulator: return "EnableSimulator";
    case MSG_DisableSimulator: return "DisableSimulator";
    case MSG_ConfirmEnableSimulator: return "ConfirmEnableSimulator";
    case MSG_TransferRequest: return "TransferRequest";
    case MSG_TransferInfo: return "TransferInfo";
    case MSG_TransferPacket: return "TransferPacket";
    case MSG_TransferAbort: return "TransferAbort";
    case MSG_RequestXfer: return "RequestXfer";
    case MSG_SendXferPacket: return "SendXferPacket";
    case MSG_ConfirmXferPacket: return "ConfirmXferPacket";
    case MSG_AbortXfer: return "AbortXfer";
    case MSG_AvatarAnimation: return "AvatarAnimation";
    case MSG_AvatarAppearance: return "AvatarAppearance";
    case MSG_AvatarSitResponse: return "AvatarSitResponse";
    case MSG_SetFollowCamProperties: return "SetFollowCamProperties";
    case MSG_ClearFollowCamProperties: return "ClearFollowCamProperties";
    case MSG_CameraConstraint: return "CameraConstraint";
    case MSG_ObjectProperties: return "ObjectProperties";
    case MSG_ObjectPropertiesFamily: return "ObjectPropertiesFamily";
    case MSG_RequestPayPrice: return "RequestPayPrice";
    case MSG_PayPriceReply: return "PayPriceReply";
    case MSG_KickUser: return "KickUser";
    case MSG_KickUserAck: return "KickUserAck";
    case MSG_GodKickUser: return "GodKickUser";
    case MSG_SystemKickUser: return "SystemKickUser";
    case MSG_EjectUser: return "EjectUser";
    case MSG_FreezeUser: return "FreezeUser";
    case MSG_AvatarPropertiesRequest: return "AvatarPropertiesRequest";
    case MSG_AvatarPropertiesRequestBackend: return "AvatarPropertiesRequestBackend";
    case MSG_AvatarPropertiesReply: return "AvatarPropertiesReply";
    case MSG_AvatarInterestsReply: return "AvatarInterestsReply";
    case MSG_AvatarGroupsReply: return "AvatarGroupsReply";
    case MSG_AvatarPropertiesUpdate: return "AvatarPropertiesUpdate";
    case MSG_AvatarInterestsUpdate: return "AvatarInterestsUpdate";
    case MSG_AvatarNotesReply: return "AvatarNotesReply";
    case MSG_AvatarNotesUpdate: return "AvatarNotesUpdate";
    case MSG_AvatarPicksReply: return "AvatarPicksReply";
    case MSG_EventInfoRequest: return "EventInfoRequest";
    case MSG_EventInfoReply: return "EventInfoReply";
    case MSG_EventNotificationAddRequest: return "EventNotificationAddRequest";
    case MSG_EventNotificationRemoveRequest: return "EventNotificationRemoveRequest";
    case MSG_EventGodDelete: return "EventGodDelete";
    case MSG_PickInfoReply: return "PickInfoReply";
    case MSG_PickInfoUpdate: return "PickInfoUpdate";
    case MSG_PickDelete: return "PickDelete";
    case MSG_PickGodDelete: return "PickGodDelete";
    case MSG_ScriptQuestion: return "ScriptQuestion";
    case MSG_ScriptControlChange: return "ScriptControlChange";
    case MSG_ScriptDialog: return "ScriptDialog";
    case MSG_ScriptDialogReply: return "ScriptDialogReply";
    case MSG_ForceScriptControlRelease: return "ForceScriptControlRelease";
    case MSG_RevokePermissions: return "RevokePermissions";
    case MSG_LoadURL: return "LoadURL";
    case MSG_ScriptTeleportRequest: return "ScriptTeleportRequest";
    case MSG_ParcelOverlay: return "ParcelOverlay";
    case MSG_ParcelPropertiesRequest: return "ParcelPropertiesRequest";
    case MSG_ParcelPropertiesRequestByID: return "ParcelPropertiesRequestByID";
    case MSG_ParcelProperties: return "ParcelProperties";
    case MSG_ParcelPropertiesUpdate: return "ParcelPropertiesUpdate";
    case MSG_ParcelReturnObjects: return "ParcelReturnObjects";
    case MSG_ParcelSetOtherCleanTime: return "ParcelSetOtherCleanTime";
    case MSG_ParcelDisableObjects: return "ParcelDisableObjects";
    case MSG_ParcelSelectObjects: return "ParcelSelectObjects";
    case MSG_EstateCovenantRequest: return "EstateCovenantRequest";
    case MSG_EstateCovenantReply: return "EstateCovenantReply";
    case MSG_ForceObjectSelect: return "ForceObjectSelect";
    case MSG_ParcelBuyPass: return "ParcelBuyPass";
    case MSG_ParcelDeedToGroup: return "ParcelDeedToGroup";
    case MSG_ParcelReclaim: return "ParcelReclaim";
    case MSG_ParcelClaim: return "ParcelClaim";
    case MSG_ParcelJoin: return "ParcelJoin";
    case MSG_ParcelDivide: return "ParcelDivide";
    case MSG_ParcelRelease: return "ParcelRelease";
    case MSG_ParcelBuy: return "ParcelBuy";
    case MSG_ParcelGodForceOwner: return "ParcelGodForceOwner";
    case MSG_ParcelAccessListRequest: return "ParcelAccessListRequest";
    case MSG_ParcelAccessListReply: return "ParcelAccessListReply";
    case MSG_ParcelAccessListUpdate: return "ParcelAccessListUpdate";
    case MSG_ParcelDwellRequest: return "ParcelDwellRequest";
    case MSG_ParcelDwellReply: return "ParcelDwellReply";
    case MSG_RequestParcelTransfer: return "RequestParcelTransfer";
    case MSG_UpdateParcel: return "UpdateParcel";
    case MSG_RemoveParcel: return "RemoveParcel";
    case MSG_MergeParcel: return "MergeParcel";
    case MSG_LogParcelChanges: return "LogParcelChanges";
    case MSG_CheckParcelSales: return "CheckParcelSales";
    case MSG_ParcelSales: return "ParcelSales";
    case MSG_ParcelGodMarkAsContent: return "ParcelGodMarkAsContent";
    case MSG_ViewerStartAuction: return "ViewerStartAuction";
    case MSG_StartAuction: return "StartAuction";
    case MSG_ConfirmAuctionStart: return "ConfirmAuctionStart";
    case MSG_CompleteAuction: return "CompleteAuction";
    case MSG_CancelAuction: return "CancelAuction";
    case MSG_CheckParcelAuctions: return "CheckParcelAuctions";
    case MSG_ParcelAuctions: return "ParcelAuctions";
    case MSG_UUIDNameRequest: return "UUIDNameRequest";
    case MSG_UUIDNameReply: return "UUIDNameReply";
    case MSG_UUIDGroupNameRequest: return "UUIDGroupNameRequest";
    case MSG_UUIDGroupNameReply: return "UUIDGroupNameReply";
    case MSG_ChatPass: return "ChatPass";
    case MSG_EdgeDataPacket: return "EdgeDataPacket";
    case MSG_SimStatus: return "SimStatus";
    case MSG_ChildAgentUpdate: return "ChildAgentUpdate";
    case MSG_ChildAgentAlive: return "ChildAgentAlive";
    case MSG_ChildAgentPositionUpdate: return "ChildAgentPositionUpdate";
    case MSG_ChildAgentDying: return "ChildAgentDying";
    case MSG_ChildAgentUnknown: return "ChildAgentUnknown";
    case MSG_AtomicPassObject: return "AtomicPassObject";
    case MSG_KillChildAgents: return "KillChildAgents";
    case MSG_GetScriptRunning: return "GetScriptRunning";
    case MSG_ScriptRunningReply: return "ScriptRunningReply";
    case MSG_SetScriptRunning: return "SetScriptRunning";
    case MSG_ScriptReset: return "ScriptReset";
    case MSG_ScriptSensorRequest: return "ScriptSensorRequest";
    case MSG_ScriptSensorReply: return "ScriptSensorReply";
    case MSG_CompleteAgentMovement: return "CompleteAgentMovement";
    case MSG_AgentMovementComplete: return "AgentMovementComplete";
    case MSG_DataServerLogout: return "DataServerLogout";
    case MSG_LogoutRequest: return "LogoutRequest";
    case MSG_LogoutReply: return "LogoutReply";
    case MSG_ImprovedInstantMessage: return "ImprovedInstantMessage";
    case MSG_RetrieveInstantMessages: return "RetrieveInstantMessages";
    case MSG_FindAgent: return "FindAgent";
    case MSG_RequestGodlikePowers: return "RequestGodlikePowers";
    case MSG_GrantGodlikePowers: return "GrantGodlikePowers";
    case MSG_GodlikeMessage: return "GodlikeMessage";
    case MSG_EstateOwnerMessage: return "EstateOwnerMessage";
    case MSG_GenericMessage: return "GenericMessage";
    case MSG_GenericStreamingMessage: return "GenericStreamingMessage";
    case MSG_LargeGenericMessage: return "LargeGenericMessage";
    case MSG_MuteListRequest: return "MuteListRequest";
    case MSG_UpdateMuteListEntry: return "UpdateMuteListEntry";
    case MSG_RemoveMuteListEntry: return "RemoveMuteListEntry";
    case MSG_CopyInventoryFromNotecard: return "CopyInventoryFromNotecard";
    case MSG_UpdateInventoryItem: return "UpdateInventoryItem";
    case MSG_UpdateCreateInventoryItem: return "UpdateCreateInventoryItem";
    case MSG_MoveInventoryItem: return "MoveInventoryItem";
    case MSG_CopyInventoryItem: return "CopyInventoryItem";
    case MSG_RemoveInventoryItem: return "RemoveInventoryItem";
    case MSG_ChangeInventoryItemFlags: return "ChangeInventoryItemFlags";
    case MSG_SaveAssetIntoInventory: return "SaveAssetIntoInventory";
    case MSG_CreateInventoryFolder: return "CreateInventoryFolder";
    case MSG_UpdateInventoryFolder: return "UpdateInventoryFolder";
    case MSG_MoveInventoryFolder: return "MoveInventoryFolder";
    case MSG_RemoveInventoryFolder: return "RemoveInventoryFolder";
    case MSG_FetchInventoryDescendents: return "FetchInventoryDescendents";
    case MSG_InventoryDescendents: return "InventoryDescendents";
    case MSG_FetchInventory: return "FetchInventory";
    case MSG_FetchInventoryReply: return "FetchInventoryReply";
    case MSG_BulkUpdateInventory: return "BulkUpdateInventory";
    case MSG_RequestInventoryAsset: return "RequestInventoryAsset";
    case MSG_InventoryAssetResponse: return "InventoryAssetResponse";
    case MSG_RemoveInventoryObjects: return "RemoveInventoryObjects";
    case MSG_PurgeInventoryDescendents: return "PurgeInventoryDescendents";
    case MSG_UpdateTaskInventory: return "UpdateTaskInventory";
    case MSG_RemoveTaskInventory: return "RemoveTaskInventory";
    case MSG_MoveTaskInventory: return "MoveTaskInventory";
    case MSG_RequestTaskInventory: return "RequestTaskInventory";
    case MSG_ReplyTaskInventory: return "ReplyTaskInventory";
    case MSG_DeRezObject: return "DeRezObject";
    case MSG_DeRezAck: return "DeRezAck";
    case MSG_RezObject: return "RezObject";
    case MSG_RezObjectFromNotecard: return "RezObjectFromNotecard";
    case MSG_TransferInventory: return "TransferInventory";
    case MSG_TransferInventoryAck: return "TransferInventoryAck";
    case MSG_AcceptFriendship: return "AcceptFriendship";
    case MSG_DeclineFriendship: return "DeclineFriendship";
    case MSG_FormFriendship: return "FormFriendship";
    case MSG_TerminateFriendship: return "TerminateFriendship";
    case MSG_OfferCallingCard: return "OfferCallingCard";
    case MSG_AcceptCallingCard: return "AcceptCallingCard";
    case MSG_DeclineCallingCard: return "DeclineCallingCard";
    case MSG_RezScript: return "RezScript";
    case MSG_CreateInventoryItem: return "CreateInventoryItem";
    case MSG_CreateLandmarkForEvent: return "CreateLandmarkForEvent";
    case MSG_EventLocationRequest: return "EventLocationRequest";
    case MSG_EventLocationReply: return "EventLocationReply";
    case MSG_RegionHandleRequest: return "RegionHandleRequest";
    case MSG_RegionIDAndHandleReply: return "RegionIDAndHandleReply";
    case MSG_MoneyTransferRequest: return "MoneyTransferRequest";
    case MSG_MoneyTransferBackend: return "MoneyTransferBackend";
    case MSG_MoneyBalanceRequest: return "MoneyBalanceRequest";
    case MSG_MoneyBalanceReply: return "MoneyBalanceReply";
    case MSG_RoutedMoneyBalanceReply: return "RoutedMoneyBalanceReply";
    case MSG_ActivateGestures: return "ActivateGestures";
    case MSG_DeactivateGestures: return "DeactivateGestures";
    case MSG_MuteListUpdate: return "MuteListUpdate";
    case MSG_UseCachedMuteList: return "UseCachedMuteList";
    case MSG_GrantUserRights: return "GrantUserRights";
    case MSG_ChangeUserRights: return "ChangeUserRights";
    case MSG_OnlineNotification: return "OnlineNotification";
    case MSG_OfflineNotification: return "OfflineNotification";
    case MSG_SetStartLocationRequest: return "SetStartLocationRequest";
    case MSG_SetStartLocation: return "SetStartLocation";
    case MSG_NetTest: return "NetTest";
    case MSG_SetCPURatio: return "SetCPURatio";
    case MSG_SimCrashed: return "SimCrashed";
    case MSG_NameValuePair: return "NameValuePair";
    case MSG_RemoveNameValuePair: return "RemoveNameValuePair";
    case MSG_UpdateAttachment: return "UpdateAttachment";
    case MSG_RemoveAttachment: return "RemoveAttachment";
    case MSG_SoundTrigger: return "SoundTrigger";
    case MSG_AttachedSound: return "AttachedSound";
    case MSG_AttachedSoundGainChange: return "AttachedSoundGainChange";
    case MSG_PreloadSound: return "PreloadSound";
    case MSG_ObjectAnimation: return "ObjectAnimation";
    case MSG_AssetUploadRequest: return "AssetUploadRequest";
    case MSG_AssetUploadComplete: return "AssetUploadComplete";
    case MSG_EmailMessageRequest: return "EmailMessageRequest";
    case MSG_EmailMessageReply: return "EmailMessageReply";
    case MSG_InternalScriptMail: return "InternalScriptMail";
    case MSG_ScriptDataRequest: return "ScriptDataRequest";
    case MSG_ScriptDataReply: return "ScriptDataReply";
    case MSG_CreateGroupRequest: return "CreateGroupRequest";
    case MSG_CreateGroupReply: return "CreateGroupReply";
    case MSG_UpdateGroupInfo: return "UpdateGroupInfo";
    case MSG_GroupRoleChanges: return "GroupRoleChanges";
    case MSG_JoinGroupRequest: return "JoinGroupRequest";
    case MSG_JoinGroupReply: return "JoinGroupReply";
    case MSG_EjectGroupMemberRequest: return "EjectGroupMemberRequest";
    case MSG_EjectGroupMemberReply: return "EjectGroupMemberReply";
    case MSG_LeaveGroupRequest: return "LeaveGroupRequest";
    case MSG_LeaveGroupReply: return "LeaveGroupReply";
    case MSG_InviteGroupRequest: return "InviteGroupRequest";
    case MSG_InviteGroupResponse: return "InviteGroupResponse";
    case MSG_GroupProfileRequest: return "GroupProfileRequest";
    case MSG_GroupProfileReply: return "GroupProfileReply";
    case MSG_GroupAccountSummaryRequest: return "GroupAccountSummaryRequest";
    case MSG_GroupAccountSummaryReply: return "GroupAccountSummaryReply";
    case MSG_GroupAccountDetailsRequest: return "GroupAccountDetailsRequest";
    case MSG_GroupAccountDetailsReply: return "GroupAccountDetailsReply";
    case MSG_GroupAccountTransactionsRequest: return "GroupAccountTransactionsRequest";
    case MSG_GroupAccountTransactionsReply: return "GroupAccountTransactionsReply";
    case MSG_GroupActiveProposalsRequest: return "GroupActiveProposalsRequest";
    case MSG_GroupActiveProposalItemReply: return "GroupActiveProposalItemReply";
    case MSG_GroupVoteHistoryRequest: return "GroupVoteHistoryRequest";
    case MSG_GroupVoteHistoryItemReply: return "GroupVoteHistoryItemReply";
    case MSG_StartGroupProposal: return "StartGroupProposal";
    case MSG_GroupProposalBallot: return "GroupProposalBallot";
    case MSG_TallyVotes: return "TallyVotes";
    case MSG_GroupMembersRequest: return "GroupMembersRequest";
    case MSG_GroupMembersReply: return "GroupMembersReply";
    case MSG_ActivateGroup: return "ActivateGroup";
    case MSG_SetGroupContribution: return "SetGroupContribution";
    case MSG_SetGroupAcceptNotices: return "SetGroupAcceptNotices";
    case MSG_GroupRoleDataRequest: return "GroupRoleDataRequest";
    case MSG_GroupRoleDataReply: return "GroupRoleDataReply";
    case MSG_GroupRoleMembersRequest: return "GroupRoleMembersRequest";
    case MSG_GroupRoleMembersReply: return "GroupRoleMembersReply";
    case MSG_GroupTitlesRequest: return "GroupTitlesRequest";
    case MSG_GroupTitlesReply: return "GroupTitlesReply";
    case MSG_GroupTitleUpdate: return "GroupTitleUpdate";
    case MSG_GroupRoleUpdate: return "GroupRoleUpdate";
    case MSG_LiveHelpGroupRequest: return "LiveHelpGroupRequest";
    case MSG_LiveHelpGroupReply: return "LiveHelpGroupReply";
    case MSG_AgentWearablesRequest: return "AgentWearablesRequest";
    case MSG_AgentWearablesUpdate: return "AgentWearablesUpdate";
    case MSG_AgentIsNowWearing: return "AgentIsNowWearing";
    case MSG_AgentCachedTexture: return "AgentCachedTexture";
    case MSG_AgentCachedTextureResponse: return "AgentCachedTextureResponse";
    case MSG_AgentDataUpdateRequest: return "AgentDataUpdateRequest";
    case MSG_AgentDataUpdate: return "AgentDataUpdate";
    case MSG_GroupDataUpdate: return "GroupDataUpdate";
    case MSG_AgentGroupDataUpdate: return "AgentGroupDataUpdate";
    case MSG_AgentDropGroup: return "AgentDropGroup";
    case MSG_LogTextMessage: return "LogTextMessage";
    case MSG_ViewerEffect: return "ViewerEffect";
    case MSG_CreateTrustedCircuit: return "CreateTrustedCircuit";
    case MSG_DenyTrustedCircuit: return "DenyTrustedCircuit";
    case MSG_RequestTrustedCircuit: return "RequestTrustedCircuit";
    case MSG_RezSingleAttachmentFromInv: return "RezSingleAttachmentFromInv";
    case MSG_RezMultipleAttachmentsFromInv: return "RezMultipleAttachmentsFromInv";
    case MSG_DetachAttachmentIntoInv: return "DetachAttachmentIntoInv";
    case MSG_CreateNewOutfitAttachments: return "CreateNewOutfitAttachments";
    case MSG_UserInfoRequest: return "UserInfoRequest";
    case MSG_UserInfoReply: return "UserInfoReply";
    case MSG_UpdateUserInfo: return "UpdateUserInfo";
    case MSG_ParcelRename: return "ParcelRename";
    case MSG_InitiateDownload: return "InitiateDownload";
    case MSG_SystemMessage: return "SystemMessage";
    case MSG_MapLayerRequest: return "MapLayerRequest";
    case MSG_MapLayerReply: return "MapLayerReply";
    case MSG_MapBlockRequest: return "MapBlockRequest";
    case MSG_MapNameRequest: return "MapNameRequest";
    case MSG_MapBlockReply: return "MapBlockReply";
    case MSG_MapItemRequest: return "MapItemRequest";
    case MSG_MapItemReply: return "MapItemReply";
    case MSG_SendPostcard: return "SendPostcard";
    case MSG_RpcChannelRequest: return "RpcChannelRequest";
    case MSG_RpcChannelReply: return "RpcChannelReply";
    case MSG_RpcScriptRequestInbound: return "RpcScriptRequestInbound";
    case MSG_RpcScriptRequestInboundForward: return "RpcScriptRequestInboundForward";
    case MSG_RpcScriptReplyInbound: return "RpcScriptReplyInbound";
    case MSG_ScriptMailRegistration: return "ScriptMailRegistration";
    case MSG_ParcelMediaCommandMessage: return "ParcelMediaCommandMessage";
    case MSG_ParcelMediaUpdate: return "ParcelMediaUpdate";
    case MSG_LandStatRequest: return "LandStatRequest";
    case MSG_LandStatReply: return "LandStatReply";
    case MSG_Error: return "Error";
    case MSG_ObjectIncludeInSearch: return "ObjectIncludeInSearch";
    case MSG_RezRestoreToWorld: return "RezRestoreToWorld";
    case MSG_LinkInventoryItem: return "LinkInventoryItem";
    case MSG_RetrieveIMsExtended: return "RetrieveIMsExtended";
    case MSG_JoinGroupRequestExtended: return "JoinGroupRequestExtended";
    case MSG_CreateGroupRequestExtended: return "CreateGroupRequestExtended";
    case MSG_GameControlInput: return "GameControlInput";
    default:
        return NULL;
    }
}

