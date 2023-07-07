/**
 * @file ICK2.h
 * @brief The interface to access the APIs provided by Virtools.
 */
#ifndef BALLOON_ICK2_H
#define BALLOON_ICK2_H

#include "CKAll.h"

namespace balloon {
    inline namespace v1 {
        class ICK2 {
        public:
            virtual CKContext *GetCKContext() const = 0;
            virtual CKRenderContext *GetRenderContext() const = 0;

            // Objects Management
            virtual CKObject *CreateObject(CK_CLASSID cid, const char *name = nullptr, CK_OBJECTCREATION_OPTIONS options = CK_OBJECTCREATION_NONAMECHECK, CK_CREATIONMODE *res = nullptr) = 0;
            virtual CKObject *CopyObject(CKObject *src, CKDependencies *dependencies = nullptr, const char *appendName = nullptr, CK_OBJECTCREATION_OPTIONS options = CK_OBJECTCREATION_NONAMECHECK) = 0;

            virtual CKObject *GetObject(CK_ID id) const = 0;
            virtual int GetObjectCount() const = 0;
            virtual int GetObjectSize(CKObject *obj) const = 0;
            virtual CKERROR DestroyObject(CKObject *obj, CKDWORD flags = 0, CKDependencies *dependencies = nullptr) = 0;
            virtual CKERROR DestroyObject(CK_ID id, CKDWORD flags = 0, CKDependencies *dependencies = nullptr) = 0;
            virtual CKERROR DestroyObjects(CK_ID *objIds, int count, CKDWORD flags = 0, CKDependencies *dependencies = nullptr) = 0;
            virtual void DestroyAllDynamicObjects() = 0;
            virtual void ChangeObjectDynamic(CKObject *obj, bool dynamic = true) = 0;

            // Object Access
            virtual CKObject *GetObjectByName(const char *name, CKObject *previous = nullptr) const = 0;
            virtual CKObject *GetObjectByNameAndClass(const char *name, CK_CLASSID cid, CKObject *previous = nullptr) const = 0;
            virtual CKObject *GetObjectByNameAndParentClass(const char *name, CK_CLASSID parentCid, CKObject *previous) const = 0;
            virtual int GetObjectsCountByClassID(CK_CLASSID cid) const = 0;
            virtual CK_ID *GetObjectsListByClassID(CK_CLASSID cid) const = 0;

            CKDataArray *GetArrayByName(const char *name) const {
                return (CKDataArray *) GetObjectByNameAndClass(name, CKCID_DATAARRAY);
            }

            CKGroup *GetGroupByName(const char *name) const {
                return (CKGroup *) GetObjectByNameAndClass(name, CKCID_GROUP);
            }

            CKMaterial *GetMaterialByName(const char *name) const {
                return (CKMaterial *) GetObjectByNameAndClass(name, CKCID_MATERIAL);
            }

            CKMesh *GetMeshByName(const char *name) const {
                return (CKMesh *) GetObjectByNameAndClass(name, CKCID_MESH);
            }

            CK2dEntity *Get2dEntityByName(const char *name) const {
                return (CK2dEntity *) GetObjectByNameAndClass(name, CKCID_2DENTITY);
            }

            CK3dEntity *Get3dEntityByName(const char *name) const {
                return (CK3dEntity *) GetObjectByNameAndClass(name, CKCID_3DENTITY);
            }

            CK3dObject *Get3dObjectByName(const char *name) const {
                return (CK3dObject *) GetObjectByNameAndClass(name, CKCID_3DOBJECT);
            }

            CKCamera *GetCameraByName(const char *name) const {
                return (CKCamera *) GetObjectByNameAndClass(name, CKCID_CAMERA);
            }

            CKTargetCamera *GetTargetCameraByName(const char *name) const {
                return (CKTargetCamera *) GetObjectByNameAndClass(name, CKCID_TARGETCAMERA);
            }

            CKLight *GetLightByName(const char *name) const {
                return (CKLight *) GetObjectByNameAndClass(name, CKCID_LIGHT);
            }

            CKTargetLight *GetTargetLightByName(const char *name) const {
                return (CKTargetLight *) GetObjectByNameAndClass(name, CKCID_TARGETLIGHT);
            }

            CKSound *GetSoundByName(const char *name) const {
                return (CKSound *) GetObjectByNameAndClass(name, CKCID_SOUND);
            }

            CKTexture *GetTextureByName(const char *name) const {
                return (CKTexture *) GetObjectByNameAndClass(name, CKCID_TEXTURE);
            }

            CKBehavior *GetScriptByName(const char *name) const {
                return (CKBehavior *) GetObjectByNameAndClass(name, CKCID_BEHAVIOR);
            }

            // Engine runtime
            virtual CKERROR Play() = 0;
            virtual CKERROR Pause() = 0;
            virtual CKERROR Reset() = 0;
            virtual bool IsPlaying() const = 0;
            virtual bool IsReset() const = 0;

            // Runtime mode
            virtual CKERROR Process() = 0;

            virtual CKERROR ClearAll() = 0;
            virtual bool IsInClearAll() const = 0;

            // Current Level & Scene functions
            virtual CKLevel *GetCurrentLevel() const = 0;
            virtual CKScene *GetCurrentScene() = 0;
            virtual void SetCurrentLevel(CKLevel *level) = 0;

            virtual void SetIC(CKBeObject *obj, bool hierarchy = false) = 0;
            virtual void RestoreIC(CKBeObject *obj, bool hierarchy = false) = 0;

            // Parameter Management
            virtual CKParameterIn *CreateCKParameterIn(const char *name, CKParameterType type, bool dynamic = false) = 0;
            virtual CKParameterIn *CreateCKParameterIn(const char *name, CKGUID guid, bool dynamic = false) = 0;
            virtual CKParameterIn *CreateCKParameterIn(const char *name, const char *type, bool dynamic = false) = 0;
            virtual CKParameterOut *CreateCKParameterOut(const char *name, CKParameterType type, bool dynamic = false) = 0;
            virtual CKParameterOut *CreateCKParameterOut(const char *name, CKGUID guid, bool dynamic = false) = 0;
            virtual CKParameterOut *CreateCKParameterOut(const char *name, const char *type, bool dynamic = false) = 0;
            virtual CKParameterLocal *CreateCKParameterLocal(const char *name, CKParameterType type, bool dynamic = false) = 0;
            virtual CKParameterLocal *CreateCKParameterLocal(const char *name, CKGUID guid, bool dynamic = false) = 0;
            virtual CKParameterLocal *CreateCKParameterLocal(const char *name, const char *type, bool dynamic = false) = 0;
            virtual CKParameterOperation *CreateCKParameterOperation(const char *name, CKGUID opguid, CKGUID resGuid, CKGUID p1Guid, CKGUID p2Guid) = 0;

            // Common Managers functions
            virtual CKBaseManager *GetManagerByGuid(CKGUID guid) const = 0;
            virtual CKBaseManager *GetManagerByName(const char *name) const = 0;

            virtual int GetManagerCount() const = 0;
            virtual CKBaseManager *GetManager(int index) const = 0;

            virtual bool IsManagerActive(CKBaseManager *manager) const = 0;
            virtual bool HasManagerDuplicates(CKBaseManager *manager) const = 0;
            virtual void ActivateManager(CKBaseManager *manager, bool active) = 0;
            virtual int GetInactiveManagerCount() const = 0;
            virtual CKBaseManager *GetInactiveManager(int index) const = 0;

            virtual CKPluginManager *GetPluginManager() const = 0;
            virtual CKParameterManager *GetParameterManager() const = 0;
            virtual CKAttributeManager *GetAttributeManager() const = 0;
            virtual CKTimeManager *GetTimeManager() const = 0;
            virtual CKMessageManager *GetMessageManager() const = 0;
            virtual CKBehaviorManager *GetBehaviorManager() const = 0;
            virtual CKPathManager *GetPathManager() const = 0;
            virtual CKRenderManager *GetRenderManager() const = 0;
            virtual CKSoundManager *GetSoundManager() const = 0;
            virtual CKInputManager *GetInputManager() const = 0;

            CKFloorManager *GetFloorManager() const {
                return (CKFloorManager *) GetManagerByGuid(FLOOR_MANAGER_GUID);
            }

            CKCollisionManager *GetCollisionManager() const {
                return (CKCollisionManager *) GetManagerByGuid(COLLISION_MANAGER_GUID);
            }

            // Profiling functions
            virtual void EnableProfiling(bool enable) = 0;
            virtual bool IsProfilingEnable() = 0;
            virtual void GetProfileStats(CKStats *stats) = 0;
            virtual void UserProfileStart(CKDWORD userSlot) = 0;
            virtual float UserProfileEnd(CKDWORD userSlot) = 0;
            virtual float GetLastUserProfileTime(CKDWORD userSlot) = 0;

            // CKFile Management
            virtual CKFile *CreateCKFile() = 0;
            virtual CKERROR DeleteCKFile(CKFile *file) = 0;

            // File Save/Load Options
            virtual int GetCompressionLevel() const = 0;
            virtual void SetCompressionLevel(int level) = 0;

            virtual CK_FILE_WRITEMODE GetFileWriteMode() const = 0;
            virtual void SetFileWriteMode(CK_FILE_WRITEMODE mode) = 0;

            virtual CK_TEXTURE_SAVEOPTIONS GetGlobalImagesSaveOptions() const = 0;
            virtual void SetGlobalImagesSaveOptions(CK_TEXTURE_SAVEOPTIONS options) = 0;

            virtual CKBitmapProperties *GetGlobalImagesSaveFormat() const = 0;
            virtual void SetGlobalImagesSaveFormat(CKBitmapProperties *format) = 0;

            virtual CK_SOUND_SAVEOPTIONS GetGlobalSoundsSaveOptions() const = 0;
            virtual void SetGlobalSoundsSaveOptions(CK_SOUND_SAVEOPTIONS options) = 0;

            // Save/Load functions
            virtual CKERROR Load(const char *filename, CKObjectArray *liste, CK_LOAD_FLAGS loadFlags = CK_LOAD_DEFAULT, CKGUID *readerGuid = nullptr) = 0;
            virtual CKERROR Load(int bufferSize, void *buffer, CKObjectArray *ckarray, CK_LOAD_FLAGS loadFlags = CK_LOAD_DEFAULT) = 0;
            virtual const char *GetLastFileLoaded() const = 0;

            virtual const char *GetLastCmoLoaded() const = 0;
            virtual void SetLastCmoLoaded(const char *str) = 0;

            virtual CKERROR GetFileInfo(const char *filename, CKFileInfo *fileInfo) const = 0;
            virtual CKERROR GetFileInfo(int bufferSize, void *buffer, CKFileInfo *fileInfo) const = 0;
            virtual CKERROR Save(const char *filename, CKObjectArray *liste, CKDWORD saveFlags, CKDependencies *dependencies = nullptr, CKGUID *readerGuid = nullptr) = 0;

            virtual void SetAutomaticLoadMode(CK_LOADMODE generalMode, CK_LOADMODE _3dObjectsMode, CK_LOADMODE meshMode, CK_LOADMODE matTexturesMode) = 0;

            virtual bool IsInLoad() = 0;
            virtual bool IsInSave() = 0;
            virtual bool IsRunTime() = 0;

            // Behavior prototype declaration functions
            virtual int GetPrototypeDeclarationCount() const = 0;
            virtual CKObjectDeclaration *GetPrototypeDeclaration(int n) const = 0;

            virtual CKObjectDeclaration *GetObjectDeclarationFromGuid(CKGUID guid) const = 0;
            virtual CKBehaviorPrototype *GetPrototypeFromGuid(CKGUID guid) const = 0;
            virtual CKERROR RemovePrototypeDeclaration(CKObjectDeclaration *decl) = 0;
            virtual CKObjectDeclaration *CreateCKObjectDeclaration(const char *name) = 0;
            virtual CKBehaviorPrototype *CreateCKBehaviorPrototype(const char *name) = 0;
            virtual CKBehaviorPrototype *CreateCKBehaviorPrototypeRunTime(const char *name) = 0;

            // Class Hierarchy Management
            virtual int GetClassCount() const = 0;
            virtual CKClassDesc *GetClassDesc(CK_CLASSID cid) const = 0;
            virtual const char *ClassIDToString(CK_CLASSID cid) const = 0;
            virtual CK_CLASSID StringToClassID(const char *classname) const = 0;

            virtual bool IsChildClassOf(CK_CLASSID child, CK_CLASSID parent) const = 0;
            virtual bool IsChildClassOf(CKObject *obj, CK_CLASSID parent) const = 0;
            virtual CK_CLASSID GetParentClassID(CK_CLASSID child) const = 0;
            virtual CK_CLASSID GetParentClassID(CKObject *obj) const = 0;
            virtual CK_CLASSID GetCommonParent(CK_CLASSID cid1, CK_CLASSID cid2) const = 0;

            // Class Dependencies Utilities
            virtual void CopyDefaultClassDependencies(CKDependencies &d, CK_DEPENDENCIES_OPMODE mode) = 0;
            virtual CKDependencies *GetDefaultClassDependencies(CK_DEPENDENCIES_OPMODE mode) = 0;

            // Merge Utilities
            virtual CKERROR CopyAllAttributes(CKBeObject *src, CKBeObject *dest) = 0;
            virtual CKERROR MoveAllScripts(CKBeObject *src, CKBeObject *dest) = 0;
            virtual CKERROR MoveScript(CKBeObject *src, CKBeObject *dest, CKBehavior *beh) = 0;
            virtual void RemapObjectParameterValue(CK_ID oldID, CK_ID newID, CK_CLASSID cid = CKCID_OBJECT, bool derived = true) = 0;

            // Array Creation Functions
            virtual CKObjectArray *CreateCKObjectArray() = 0;
            virtual void DeleteCKObjectArray(CKObjectArray *obj) = 0;

            // StateChunk Creation Functions
            virtual CKStateChunk *CreateCKStateChunk(CK_CLASSID id, CKFile *file = nullptr) = 0;
            virtual CKStateChunk *CreateCKStateChunk(CKStateChunk *chunk) = 0;
            virtual void DeleteCKStateChunk(CKStateChunk *chunk) = 0;

            virtual CKStateChunk *SaveObjectState(CKObject *obj, CKDWORD flags = CK_STATESAVE_ALL) = 0;
            virtual CKERROR ReadObjectState(CKObject *obj, CKStateChunk *chunk) = 0;

            // Bitmap utilities
            virtual BITMAP_HANDLE LoadBitmap(const char *filename) = 0;
            virtual bool SaveBitmap(const char *filename, BITMAP_HANDLE bm) = 0;
            virtual bool SaveBitmap(const char *filename, VxImageDescEx &desc) = 0;

            // Bitmap Properties Utilities
            virtual CKBitmapProperties *CopyBitmapProperties(CKBitmapProperties *bp) = 0;
            virtual void DeleteBitmapProperties(CKBitmapProperties *bp) = 0;

            //--- Endian Conversion utilities
            virtual void ConvertEndianArray32(void *buf, int dwordCount) = 0;
            virtual void ConvertEndianArray16(void *buf, int dwordCount) = 0;
            virtual CKDWORD ConvertEndian32(CKDWORD dw) = 0;
            virtual CKWORD ConvertEndian16(CKWORD w) = 0;

            // Compression utilities
            virtual CKDWORD ComputeDataCRC(const char *data, int size, CKDWORD prevCRC = 0) = 0;
            virtual char *PackData(const char *Data, int size, int &newSize, int compressionLevel) = 0;
            virtual char *UnPackData(int destSize, char *srcBuffer, int srcSize) = 0;

            // Memory Management Utilities
            virtual void *MallocAligned(int size, int align) = 0;
            virtual void FreeAligned(void *ptr) = 0;

            // String Utilities
            virtual char *Strdup(const char *str) = 0;
            virtual char *Strupr(const char *str) = 0;
            virtual char *Strlwr(const char *str) = 0;

            // Utils
            virtual CKGUID GetSecureGuid() = 0;
            virtual CKDWORD GetStartOptions() = 0;
            virtual WIN_HANDLE GetMainWindow() = 0;
            virtual int GetSelectedRenderEngine() = 0;

            virtual const char *GetStartPath() const = 0;
            virtual const char *GetPluginsPath() const = 0;
            virtual CKDWORD GetVersion() const = 0;

            // IHM
            virtual void SetInterfaceMode(bool mode = true, CKUICALLBACKFCT callback = nullptr, void *data = nullptr) = 0;
            virtual bool IsInInterfaceMode() const = 0;

            virtual CKERROR OutputToConsole(const char *str, bool beep = true) = 0;
            virtual CKERROR OutputToConsoleEx(const char *format, ...) = 0;
            virtual CKERROR OutputToConsoleExBeep(const char *format, ...) = 0;
            virtual CKDWORD SendInterfaceMessage(CKDWORD reason, CKDWORD param1, CKDWORD param2) = 0;

            virtual void SetVirtoolsVersion(CK_VIRTOOLS_VERSION ver, CKDWORD build) = 0;
            virtual int GetPVInformation() const = 0;
            virtual bool IsInDynamicCreationMode() const = 0;

            // User data
            virtual void *GetUserData(size_t type = 0) const = 0;
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_ICK2_H
