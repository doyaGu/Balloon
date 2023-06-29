#ifndef BALLOON_CK2_H
#define BALLOON_CK2_H

#include "Balloon/ICK2.h"
#include "Balloon/DataBox.h"

namespace balloon {
    class CK2 final : public ICK2 {
    public:
        static CK2 &GetInstance();

        CK2(const CK2 &rhs) = delete;
        CK2(CK2 &&rhs) noexcept = delete;

        ~CK2();

        CK2 &operator=(const CK2 &rhs) = delete;
        CK2 &operator=(CK2 &&rhs) noexcept = delete;

        bool Init(CKContext *context);
        void Shutdown();

        CKContext *GetCKContext() const override;
        CKRenderContext *GetRenderContext() const override;

        CKObject *CreateObject(CK_CLASSID cid, const char *name, CK_OBJECTCREATION_OPTIONS options, CK_CREATIONMODE *res) override;
        CKObject *CopyObject(CKObject *src, CKDependencies *dependencies, const char *appendName, CK_OBJECTCREATION_OPTIONS options) override;
        CKObject *GetObject(CK_ID id) const override;
        int GetObjectCount() const override;
        int GetObjectSize(CKObject *obj) const override;

        CKERROR DestroyObject(CKObject *obj, CKDWORD flags, CKDependencies *dependencies) override;
        CKERROR DestroyObject(CK_ID id, CKDWORD flags, CKDependencies *dependencies) override;
        CKERROR DestroyObjects(CK_ID *objIds, int count, CKDWORD flags, CKDependencies *dependencies) override;
        void DestroyAllDynamicObjects() override;

        void ChangeObjectDynamic(CKObject *obj, bool dynamic) override;

        CKObject *GetObjectByName(const char *name, CKObject *previous) const override;
        CKObject *GetObjectByNameAndClass(const char *name, CK_CLASSID cid, CKObject *previous) const override;
        CKObject *GetObjectByNameAndParentClass(const char *name, CK_CLASSID parentCid, CKObject *previous) const override;
        int GetObjectsCountByClassID(CK_CLASSID cid) const override;
        CK_ID *GetObjectsListByClassID(CK_CLASSID cid) const override;

        CKERROR Play() override;
        CKERROR Pause() override;
        CKERROR Reset() override;

        bool IsPlaying() const override;
        bool IsReset() const override;

        CKERROR Process() override;
        CKERROR ClearAll() override;
        bool IsInClearAll() const override;

        CKLevel *GetCurrentLevel() const override;
        CKScene *GetCurrentScene() override;
        void SetCurrentLevel(CKLevel *level) override;

        void SetIC(CKBeObject *obj, bool hierarchy) override;
        void RestoreIC(CKBeObject *obj, bool hierarchy) override;

        CKParameterIn *CreateCKParameterIn(const char *name, CKParameterType type, bool dynamic) override;
        CKParameterIn *CreateCKParameterIn(const char *name, CKGUID guid, bool dynamic) override;
        CKParameterIn *CreateCKParameterIn(const char *name, const char *type, bool dynamic) override;
        CKParameterOut *CreateCKParameterOut(const char *name, CKParameterType type, bool dynamic) override;
        CKParameterOut *CreateCKParameterOut(const char *name, CKGUID guid, bool dynamic) override;
        CKParameterOut *CreateCKParameterOut(const char *name, const char *type, bool dynamic) override;
        CKParameterLocal *CreateCKParameterLocal(const char *name, CKParameterType type, bool dynamic) override;
        CKParameterLocal *CreateCKParameterLocal(const char *name, CKGUID guid, bool dynamic) override;
        CKParameterLocal *CreateCKParameterLocal(const char *name, const char *type, bool dynamic) override;
        CKParameterOperation *CreateCKParameterOperation(const char *name, CKGUID opguid, CKGUID resGuid, CKGUID p1Guid, CKGUID p2Guid) override;

        CKBaseManager *GetManagerByGuid(CKGUID guid) const override;
        CKBaseManager *GetManagerByName(const char *name) const override;
        int GetManagerCount() const override;
        CKBaseManager *GetManager(int index) const override;
        bool IsManagerActive(CKBaseManager *manager) const override;
        bool HasManagerDuplicates(CKBaseManager *manager) const override;
        void ActivateManager(CKBaseManager *manager, bool active) override;
        int GetInactiveManagerCount() const override;
        CKBaseManager *GetInactiveManager(int index) const override;
        CKPluginManager *GetPluginManager() const override;
        CKObjectManager *GetObjectManager() const override;
        CKParameterManager *GetParameterManager() const override;
        CKAttributeManager *GetAttributeManager() const override;
        CKTimeManager *GetTimeManager() const override;
        CKMessageManager *GetMessageManager() const override;
        CKBehaviorManager *GetBehaviorManager() const override;
        CKPathManager *GetPathManager() const override;
        CKRenderManager *GetRenderManager() const override;
        CKSoundManager *GetSoundManager() const override;
        CKInputManager *GetInputManager() const override;

        void EnableProfiling(bool enable) override;
        bool IsProfilingEnable() override;

        void GetProfileStats(CKStats *stats) override;
        void UserProfileStart(CKDWORD userSlot) override;
        float UserProfileEnd(CKDWORD userSlot) override;
        float GetLastUserProfileTime(CKDWORD userSlot) override;

        CKFile *CreateCKFile() override;
        CKERROR DeleteCKFile(CKFile *file) override;

        int GetCompressionLevel() const override;
        void SetCompressionLevel(int level) override;

        CK_FILE_WRITEMODE GetFileWriteMode() const override;
        void SetFileWriteMode(CK_FILE_WRITEMODE mode) override;

        CK_TEXTURE_SAVEOPTIONS GetGlobalImagesSaveOptions() const override;
        void SetGlobalImagesSaveOptions(CK_TEXTURE_SAVEOPTIONS options) override;

        CKBitmapProperties *GetGlobalImagesSaveFormat() const override;
        void SetGlobalImagesSaveFormat(CKBitmapProperties *format) override;

        CK_SOUND_SAVEOPTIONS GetGlobalSoundsSaveOptions() const override;
        void SetGlobalSoundsSaveOptions(CK_SOUND_SAVEOPTIONS options) override;

        CKERROR Load(const char *filename, CKObjectArray *liste, CK_LOAD_FLAGS loadFlags, CKGUID *readerGuid) override;
        CKERROR Load(int bufferSize, void *buffer, CKObjectArray *ckarray, CK_LOAD_FLAGS loadFlags) override;

        const char *GetLastFileLoaded() const override;
        const char *GetLastCmoLoaded() const override;
        void SetLastCmoLoaded(const char *str) override;

        CKERROR GetFileInfo(const char *filename, CKFileInfo *fileInfo) const override;
        CKERROR GetFileInfo(int bufferSize, void *buffer, CKFileInfo *fileInfo) const override;

        CKERROR Save(const char *filename, CKObjectArray *liste, CKDWORD saveFlags, CKDependencies *dependencies, CKGUID *readerGuid) override;

        void SetAutomaticLoadMode(CK_LOADMODE generalMode, CK_LOADMODE _3dObjectsMode, CK_LOADMODE meshMode, CK_LOADMODE matTexturesMode) override;
        void SetUserLoadCallback(CK_USERLOADCALLBACK callback, void *arg) override;
        CK_LOADMODE LoadVerifyObjectUnicity(const char *oldName, CK_CLASSID cid, const char *newName, CKObject **newObj) override;

        bool IsInLoad() override;
        bool IsInSave() override;
        bool IsRunTime() override;

        int GetPrototypeDeclarationCount() const override;
        CKObjectDeclaration *GetPrototypeDeclaration(int n) const override;
        CKObjectDeclaration *GetObjectDeclarationFromGuid(CKGUID guid) const override;
        CKBehaviorPrototype *GetPrototypeFromGuid(CKGUID guid) const override;
        CKERROR RemovePrototypeDeclaration(CKObjectDeclaration *decl) override;
        CKObjectDeclaration *CreateCKObjectDeclaration(const char *name) override;
        CKBehaviorPrototype *CreateCKBehaviorPrototype(const char *name) override;
        CKBehaviorPrototype *CreateCKBehaviorPrototypeRunTime(const char *name) override;

        int GetClassCount() const override;
        CKClassDesc *GetClassDesc(CK_CLASSID cid) const override;
        const char *ClassIDToString(CK_CLASSID cid) const override;
        CK_CLASSID StringToClassID(const char *classname) const override;
        bool IsChildClassOf(CK_CLASSID child, CK_CLASSID parent) const override;
        bool IsChildClassOf(CKObject *obj, CK_CLASSID parent) const override;
        CK_CLASSID GetParentClassID(CK_CLASSID child) const override;
        CK_CLASSID GetParentClassID(CKObject *obj) const override;
        CK_CLASSID GetCommonParent(CK_CLASSID cid1, CK_CLASSID cid2) const override;

        void CopyDefaultClassDependencies(CKDependencies &d, CK_DEPENDENCIES_OPMODE mode) override;
        CKDependencies *GetDefaultClassDependencies(CK_DEPENDENCIES_OPMODE mode) override;

        CKERROR CopyAllAttributes(CKBeObject *src, CKBeObject *dest) override;
        CKERROR MoveAllScripts(CKBeObject *src, CKBeObject *dest) override;
        CKERROR MoveScript(CKBeObject *src, CKBeObject *dest, CKBehavior *beh) override;
        void RemapObjectParameterValue(CK_ID oldID, CK_ID newID, CK_CLASSID cid, bool derived) override;

        CKObjectArray *CreateCKObjectArray() override;
        void DeleteCKObjectArray(CKObjectArray *obj) override;

        CKStateChunk *CreateCKStateChunk(CK_CLASSID id, CKFile *file) override;
        CKStateChunk *CreateCKStateChunk(CKStateChunk *chunk) override;
        void DeleteCKStateChunk(CKStateChunk *chunk) override;

        CKStateChunk *SaveObjectState(CKObject *obj, CKDWORD flags) override;
        CKERROR ReadObjectState(CKObject *obj, CKStateChunk *chunk) override;

        BITMAP_HANDLE LoadBitmap(const char *filename) override;
        bool SaveBitmap(const char *filename, BITMAP_HANDLE bm) override;
        bool SaveBitmap(const char *filename, VxImageDescEx &desc) override;

        CKBitmapProperties *CopyBitmapProperties(CKBitmapProperties *bp) override;
        void DeleteBitmapProperties(CKBitmapProperties *bp) override;

        void ConvertEndianArray32(void *buf, int dwordCount) override;
        void ConvertEndianArray16(void *buf, int dwordCount) override;
        CKDWORD ConvertEndian32(CKDWORD dw) override;
        CKWORD ConvertEndian16(CKWORD w) override;
        CKDWORD ComputeDataCRC(const char *data, int size, CKDWORD prevCRC) override;

        char *PackData(const char *Data, int size, int &newSize, int compressionLevel) override;
        char *UnPackData(int destSize, char *srcBuffer, int srcSize) override;

        void *Malloc(unsigned int n) override;
        void Free(void *ptr) override;
        void *MallocAligned(int size, int align) override;
        void FreeAligned(void *ptr) override;

        char *Strdup(const char *str) override;
        char *Strupr(const char *str) override;
        char *Strlwr(const char *str) override;

        char *GetStringBuffer(int size) override;
        CKGUID GetSecureGuid() override;
        CKDWORD GetStartOptions() override;
        WIN_HANDLE GetMainWindow() override;
        int GetSelectedRenderEngine() override;
        const char *GetStartPath() const override;
        const char *GetPluginsPath() const override;
        CKDWORD GetVersion() const override;

        void SetInterfaceMode(bool mode, CKUICALLBACKFCT callback, void *data) override;
        bool IsInInterfaceMode() const override;

        CKERROR OutputToConsole(const char *str, bool beep) override;
        CKERROR OutputToConsoleEx(const char *format, ...) override;
        CKERROR OutputToConsoleExBeep(const char *format, ...) override;
        CKDWORD SendInterfaceMessage(CKDWORD reason, CKDWORD param1, CKDWORD param2) override;

        void SetVirtoolsVersion(CK_VIRTOOLS_VERSION ver, CKDWORD build) override;
        int GetPVInformation() const override;
        bool IsInDynamicCreationMode() const override;

        void *GetUserData(size_t type) const override;
        void *SetUserData(void *data, size_t type) override;

    private:
        CK2();

        CKContext *m_CKContext = nullptr;
        DataBox m_UserData;
    };
}

#endif // BALLOON_CK2_H
