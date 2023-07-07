#include "CK2.h"

#include <cassert>

using namespace balloon;

CK2 &CK2::GetInstance() {
    static CK2 instance;
    return instance;
}

CK2::~CK2() = default;

bool CK2::Init(CKContext *context) {
    if (!context)
        return false;
    m_CKContext = context;
    return true;
}

void CK2::Shutdown() {}

CKContext *CK2::GetCKContext() const {
    return m_CKContext;
}

CKRenderContext *CK2::GetRenderContext() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetPlayerRenderContext();
}

CKObject *CK2::CreateObject(CK_CLASSID cid, const char *name, CK_OBJECTCREATION_OPTIONS options, CK_CREATIONMODE *res) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateObject(cid, const_cast<CKSTRING>(name), options, res);
}

CKObject *CK2::CopyObject(CKObject *src, CKDependencies *dependencies, const char *appendName, CK_OBJECTCREATION_OPTIONS options) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CopyObject(src, dependencies, const_cast<CKSTRING>(appendName), options);
}

CKObject *CK2::GetObject(CK_ID id) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObject(id);
}

int CK2::GetObjectCount() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectCount();
}

int CK2::GetObjectSize(CKObject *obj) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectSize(obj);
}

CKERROR CK2::DestroyObject(CKObject *obj, CKDWORD flags, CKDependencies *dependencies) {
    assert(m_CKContext != nullptr);
    return m_CKContext->DestroyObject(obj, flags, dependencies);
}

CKERROR CK2::DestroyObject(CK_ID id, CKDWORD flags, CKDependencies *dependencies) {
    assert(m_CKContext != nullptr);
    return m_CKContext->DestroyObject(id, flags, dependencies);
}

CKERROR CK2::DestroyObjects(CK_ID *objIds, int count, CKDWORD flags, CKDependencies *dependencies) {
    assert(m_CKContext != nullptr);
    return m_CKContext->DestroyObjects(objIds, count, flags, dependencies);
}

void CK2::DestroyAllDynamicObjects() {
    assert(m_CKContext != nullptr);
    m_CKContext->DestroyAllDynamicObjects();
}

void CK2::ChangeObjectDynamic(CKObject *obj, bool dynamic) {
    assert(m_CKContext != nullptr);
    m_CKContext->ChangeObjectDynamic(obj, dynamic);
}

CKObject *CK2::GetObjectByName(const char *name, CKObject *previous) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectByName(const_cast<CKSTRING>(name), previous);
}

CKObject *CK2::GetObjectByNameAndClass(const char *name, CK_CLASSID cid, CKObject *previous) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectByNameAndClass(const_cast<CKSTRING>(name), cid, previous);
}

CKObject *CK2::GetObjectByNameAndParentClass(const char *name, CK_CLASSID parentCid, CKObject *previous) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectByNameAndParentClass(const_cast<CKSTRING>(name), parentCid, previous);
}

int CK2::GetObjectsCountByClassID(CK_CLASSID cid) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectsCountByClassID(cid);
}

CK_ID *CK2::GetObjectsListByClassID(CK_CLASSID cid) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetObjectsListByClassID(cid);
}

CKERROR CK2::Play() {
    assert(m_CKContext != nullptr);
    return m_CKContext->Play();
}

CKERROR CK2::Pause() {
    assert(m_CKContext != nullptr);
    return m_CKContext->Pause();
}

CKERROR CK2::Reset() {
    assert(m_CKContext != nullptr);
    return m_CKContext->Reset();
}

bool CK2::IsPlaying() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsPlaying();
}

bool CK2::IsReset() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsReseted();
}

CKERROR CK2::Process() {
    assert(m_CKContext != nullptr);
    return m_CKContext->Process();
}

CKERROR CK2::ClearAll() {
    assert(m_CKContext != nullptr);
    return m_CKContext->ClearAll();
}

bool CK2::IsInClearAll() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsInClearAll();
}

CKLevel *CK2::GetCurrentLevel() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetCurrentLevel();
}

CKScene *CK2::GetCurrentScene() {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetCurrentScene();
}

void CK2::SetCurrentLevel(CKLevel *level) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetCurrentLevel(level);
}

void CK2::SetIC(CKBeObject *obj, bool hierarchy) {
    GetCurrentScene()->SetObjectInitialValue(obj, CKSaveObjectState(obj));
    if (hierarchy) {
        if (CKIsChildClassOf(obj, CKCID_2DENTITY)) {
            auto *entity = (CK2dEntity *) obj;
            for (int i = 0; i < entity->GetChildrenCount(); i++)
                SetIC(entity->GetChild(i), true);
        }
        if (CKIsChildClassOf(obj, CKCID_3DENTITY)) {
            auto *entity = (CK3dEntity *) obj;
            for (int i = 0; i < entity->GetChildrenCount(); i++)
                SetIC(entity->GetChild(i), true);
        }
    }
}

void CK2::RestoreIC(CKBeObject *obj, bool hierarchy) {
    CKStateChunk *chunk = GetCurrentScene()->GetObjectInitialValue(obj);
    if (chunk)
        CKReadObjectState(obj, chunk);

    if (hierarchy) {
        if (CKIsChildClassOf(obj, CKCID_2DENTITY)) {
            auto *entity = (CK2dEntity *) obj;
            for (int i = 0; i < entity->GetChildrenCount(); i++)
                RestoreIC(entity->GetChild(i), true);
        }
        if (CKIsChildClassOf(obj, CKCID_3DENTITY)) {
            auto *entity = (CK3dEntity *) obj;
            for (int i = 0; i < entity->GetChildrenCount(); i++)
                RestoreIC(entity->GetChild(i), true);
        }
    }
}

CKParameterIn *CK2::CreateCKParameterIn(const char *name, CKParameterType type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterIn(const_cast<CKSTRING>(name), type, dynamic);
}

CKParameterIn *CK2::CreateCKParameterIn(const char *name, CKGUID guid, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterIn(const_cast<CKSTRING>(name), guid, dynamic);
}

CKParameterIn *CK2::CreateCKParameterIn(const char *name, const char *type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterIn(const_cast<CKSTRING>(name), const_cast<CKSTRING>(type), dynamic);
}

CKParameterOut *CK2::CreateCKParameterOut(const char *name, CKParameterType type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterOut(const_cast<CKSTRING>(name), type, dynamic);
}

CKParameterOut *CK2::CreateCKParameterOut(const char *name, CKGUID guid, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterOut(const_cast<CKSTRING>(name), guid, dynamic);
}

CKParameterOut *CK2::CreateCKParameterOut(const char *name, const char *type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterOut(const_cast<CKSTRING>(name), const_cast<CKSTRING>(type), dynamic);
}

CKParameterLocal *CK2::CreateCKParameterLocal(const char *name, CKParameterType type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterLocal(const_cast<CKSTRING>(name), type, dynamic);
}

CKParameterLocal *CK2::CreateCKParameterLocal(const char *name, CKGUID guid, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterLocal(const_cast<CKSTRING>(name), guid, dynamic);
}

CKParameterLocal *CK2::CreateCKParameterLocal(const char *name, const char *type, bool dynamic) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterLocal(const_cast<CKSTRING>(name), const_cast<CKSTRING>(type), dynamic);
}

CKParameterOperation *CK2::CreateCKParameterOperation(const char *name, CKGUID opguid, CKGUID resGuid, CKGUID p1Guid, CKGUID p2Guid) {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKParameterOperation(const_cast<CKSTRING>(name), opguid, resGuid, p1Guid, p2Guid);
}

CKBaseManager *CK2::GetManagerByGuid(CKGUID guid) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetManagerByGuid(guid);
}

CKBaseManager *CK2::GetManagerByName(const char *name) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetManagerByName(const_cast<CKSTRING>(name));
}

int CK2::GetManagerCount() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetManagerCount();
}

CKBaseManager *CK2::GetManager(int index) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetManager(index);
}

bool CK2::IsManagerActive(CKBaseManager *manager) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsManagerActive(manager);
}

bool CK2::HasManagerDuplicates(CKBaseManager *manager) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->HasManagerDuplicates(manager);
}

void CK2::ActivateManager(CKBaseManager *manager, bool active) {
    assert(m_CKContext != nullptr);
    m_CKContext->ActivateManager(manager, active);
}

int CK2::GetInactiveManagerCount() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetInactiveManagerCount();
}

CKBaseManager *CK2::GetInactiveManager(int index) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetInactiveManager(index);
}

CKPluginManager *CK2::GetPluginManager() const {
    return CKGetPluginManager();
}

CKObjectManager *CK2::GetObjectManager() const {
    assert(m_CKContext != nullptr);
    return (CKObjectManager *) m_CKContext->GetManagerByGuid(OBJECT_MANAGER_GUID);
}

CKParameterManager *CK2::GetParameterManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetParameterManager();
}

CKAttributeManager *CK2::GetAttributeManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetAttributeManager();
}

CKTimeManager *CK2::GetTimeManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetTimeManager();
}

CKMessageManager *CK2::GetMessageManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetMessageManager();
}

CKBehaviorManager *CK2::GetBehaviorManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetBehaviorManager();
}

CKPathManager *CK2::GetPathManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetPathManager();
}

CKRenderManager *CK2::GetRenderManager() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetRenderManager();
}

CKSoundManager *CK2::GetSoundManager() const {
    assert(m_CKContext != nullptr);
    return (CKSoundManager *) m_CKContext->GetManagerByGuid(SOUND_MANAGER_GUID);
}

CKInputManager *CK2::GetInputManager() const {
    assert(m_CKContext != nullptr);
    return (CKInputManager *) m_CKContext->GetManagerByGuid(INPUT_MANAGER_GUID);
}

void CK2::EnableProfiling(bool enable) {
    assert(m_CKContext != nullptr);
    m_CKContext->EnableProfiling(enable);
}

bool CK2::IsProfilingEnable() {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsProfilingEnable();
}

void CK2::GetProfileStats(CKStats *stats) {
    assert(m_CKContext != nullptr);
    m_CKContext->GetProfileStats(stats);
}

void CK2::UserProfileStart(CKDWORD userSlot) {
    assert(m_CKContext != nullptr);
    m_CKContext->UserProfileStart(userSlot);
}

float CK2::UserProfileEnd(CKDWORD userSlot) {
    assert(m_CKContext != nullptr);
    return m_CKContext->UserProfileEnd(userSlot);
}

float CK2::GetLastUserProfileTime(CKDWORD userSlot) {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetLastUserProfileTime(userSlot);
}

CKFile *CK2::CreateCKFile() {
    assert(m_CKContext != nullptr);
    return m_CKContext->CreateCKFile();
}

CKERROR CK2::DeleteCKFile(CKFile *file) {
    assert(m_CKContext != nullptr);
    return m_CKContext->DeleteCKFile(file);
}

int CK2::GetCompressionLevel() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetCompressionLevel();
}

void CK2::SetCompressionLevel(int level) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetCompressionLevel(level);
}

CK_FILE_WRITEMODE CK2::GetFileWriteMode() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetFileWriteMode();
}

void CK2::SetFileWriteMode(CK_FILE_WRITEMODE mode) {
    m_CKContext->SetFileWriteMode(mode);
}

CK_TEXTURE_SAVEOPTIONS CK2::GetGlobalImagesSaveOptions() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetGlobalImagesSaveOptions();
}

void CK2::SetGlobalImagesSaveOptions(CK_TEXTURE_SAVEOPTIONS options) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetGlobalImagesSaveOptions(options);
}

CKBitmapProperties *CK2::GetGlobalImagesSaveFormat() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetGlobalImagesSaveFormat();
}

void CK2::SetGlobalImagesSaveFormat(CKBitmapProperties *format) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetGlobalImagesSaveFormat(format);
}

CK_SOUND_SAVEOPTIONS CK2::GetGlobalSoundsSaveOptions() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetGlobalSoundsSaveOptions();
}

void CK2::SetGlobalSoundsSaveOptions(CK_SOUND_SAVEOPTIONS options) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetGlobalSoundsSaveOptions(options);
}

CKERROR CK2::Load(const char *filename, CKObjectArray *liste, CK_LOAD_FLAGS loadFlags, CKGUID *readerGuid) {
    assert(m_CKContext != nullptr);
    return m_CKContext->Load(const_cast<CKSTRING>(filename), liste, loadFlags, readerGuid);
}

CKERROR CK2::Load(int bufferSize, void *buffer, CKObjectArray *ckarray, CK_LOAD_FLAGS loadFlags) {
    assert(m_CKContext != nullptr);
    return m_CKContext->Load(bufferSize, buffer, ckarray, loadFlags);
}

const char *CK2::GetLastFileLoaded() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetLastFileLoaded();
}

const char *CK2::GetLastCmoLoaded() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetLastCmoLoaded();;
}

void CK2::SetLastCmoLoaded(const char *str) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetLastCmoLoaded(const_cast<CKSTRING>(str));
}

CKERROR CK2::GetFileInfo(const char *filename, CKFileInfo *fileInfo) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetFileInfo(const_cast<CKSTRING>(filename), fileInfo);
}

CKERROR CK2::GetFileInfo(int bufferSize, void *buffer, CKFileInfo *fileInfo) const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetFileInfo(bufferSize, buffer, fileInfo);
}

CKERROR CK2::Save(const char *filename, CKObjectArray *liste, CKDWORD saveFlags, CKDependencies *dependencies, CKGUID *readerGuid) {
    assert(m_CKContext != nullptr);
    return m_CKContext->Save(const_cast<CKSTRING>(filename), liste, saveFlags, dependencies, readerGuid);
}

void CK2::SetAutomaticLoadMode(CK_LOADMODE generalMode, CK_LOADMODE _3dObjectsMode, CK_LOADMODE meshMode, CK_LOADMODE matTexturesMode) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetAutomaticLoadMode(generalMode, _3dObjectsMode, meshMode, matTexturesMode);
}

void CK2::SetUserLoadCallback(CK_USERLOADCALLBACK callback, void *arg) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetUserLoadCallback(callback, arg);
}

CK_LOADMODE CK2::LoadVerifyObjectUnicity(const char *oldName, CK_CLASSID cid, const char *newName, CKObject **newObj) {
    assert(m_CKContext != nullptr);
    return m_CKContext->LoadVerifyObjectUnicity(const_cast<CKSTRING>(oldName), cid, const_cast<CKSTRING>(newName), newObj);
}

bool CK2::IsInLoad() {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsInLoad();
}

bool CK2::IsInSave() {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsInSave();
}

bool CK2::IsRunTime() {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsRunTime();
}

int CK2::GetPrototypeDeclarationCount() const {
    return ::CKGetPrototypeDeclarationCount();
}

CKObjectDeclaration *CK2::GetPrototypeDeclaration(int n) const {
    return ::CKGetPrototypeDeclaration(n);
}

CKObjectDeclaration *CK2::GetObjectDeclarationFromGuid(CKGUID guid) const {
    return ::CKGetObjectDeclarationFromGuid(guid);
}

CKBehaviorPrototype *CK2::GetPrototypeFromGuid(CKGUID guid) const {
    return ::CKGetPrototypeFromGuid(guid);
}

CKERROR CK2::RemovePrototypeDeclaration(CKObjectDeclaration *decl) {
    return ::CKRemovePrototypeDeclaration(decl);
}

CKObjectDeclaration *CK2::CreateCKObjectDeclaration(const char *name) {
    return ::CreateCKObjectDeclaration(const_cast<CKSTRING>(name));
}

CKBehaviorPrototype *CK2::CreateCKBehaviorPrototype(const char *name) {
    return ::CreateCKBehaviorPrototype(const_cast<CKSTRING>(name));
}

CKBehaviorPrototype *CK2::CreateCKBehaviorPrototypeRunTime(const char *name) {
    return ::CreateCKBehaviorPrototypeRunTime(const_cast<CKSTRING>(name));
}

int CK2::GetClassCount() const {
    return ::CKGetClassCount();
}

CKClassDesc *CK2::GetClassDesc(CK_CLASSID cid) const {
    return ::CKGetClassDesc(cid);
}

const char *CK2::ClassIDToString(CK_CLASSID cid) const {
    return ::CKClassIDToString(cid);
}

CK_CLASSID CK2::StringToClassID(const char *classname) const {
    return ::CKStringToClassID(const_cast<CKSTRING>(classname));
}

bool CK2::IsChildClassOf(CK_CLASSID child, CK_CLASSID parent) const {
    return ::CKIsChildClassOf(child, parent);
}

bool CK2::IsChildClassOf(CKObject *obj, CK_CLASSID parent) const {
    return ::CKIsChildClassOf(obj, parent);
}

CK_CLASSID CK2::GetParentClassID(CK_CLASSID child) const {
    return ::CKGetParentClassID(child);
}

CK_CLASSID CK2::GetParentClassID(CKObject *obj) const {
    return ::CKGetParentClassID(obj);
}

CK_CLASSID CK2::GetCommonParent(CK_CLASSID cid1, CK_CLASSID cid2) const {
    return ::CKGetCommonParent(cid1, cid2);
}

void CK2::CopyDefaultClassDependencies(CKDependencies &d, CK_DEPENDENCIES_OPMODE mode) {
    ::CKCopyDefaultClassDependencies(d, mode);
}

CKDependencies *CK2::GetDefaultClassDependencies(CK_DEPENDENCIES_OPMODE mode) {
    return ::CKGetDefaultClassDependencies(mode);
}

CKERROR CK2::CopyAllAttributes(CKBeObject *src, CKBeObject *dest) {
    return ::CKCopyAllAttributes(src, dest);
}

CKERROR CK2::MoveAllScripts(CKBeObject *src, CKBeObject *dest) {
    return ::CKMoveAllScripts(src, dest);
}

CKERROR CK2::MoveScript(CKBeObject *src, CKBeObject *dest, CKBehavior *beh) {
    return ::CKMoveScript(src, dest, beh);
}

void CK2::RemapObjectParameterValue(CK_ID oldID, CK_ID newID, CK_CLASSID cid, bool derived) {
    ::CKRemapObjectParameterValue(m_CKContext, oldID, newID, cid, derived);
}

CKObjectArray *CK2::CreateCKObjectArray() {
    return ::CreateCKObjectArray();
}

void CK2::DeleteCKObjectArray(CKObjectArray *obj) {
    return ::DeleteCKObjectArray(obj);
}

CKStateChunk *CK2::CreateCKStateChunk(CK_CLASSID id, CKFile *file) {
    return ::CreateCKStateChunk(id, file);
}

CKStateChunk *CK2::CreateCKStateChunk(CKStateChunk *chunk) {
    return ::CreateCKStateChunk(chunk);
}

void CK2::DeleteCKStateChunk(CKStateChunk *chunk) {
    ::DeleteCKStateChunk(chunk);
}

CKStateChunk *CK2::SaveObjectState(CKObject *obj, CKDWORD flags) {
    return ::CKSaveObjectState(obj, flags);
}

CKERROR CK2::ReadObjectState(CKObject *obj, CKStateChunk *chunk) {
    return ::CKReadObjectState(obj, chunk);
}

BITMAP_HANDLE CK2::LoadBitmap(const char *filename) {
    return ::CKLoadBitmap(const_cast<CKSTRING>(filename));
}

bool CK2::SaveBitmap(const char *filename, BITMAP_HANDLE bm) {
    return ::CKSaveBitmap(const_cast<CKSTRING>(filename), bm);
}

bool CK2::SaveBitmap(const char *filename, VxImageDescEx &desc) {
    return ::CKSaveBitmap(const_cast<CKSTRING>(filename), desc);
}

CKBitmapProperties *CK2::CopyBitmapProperties(CKBitmapProperties *bp) {
    return ::CKCopyBitmapProperties(bp);
}

void CK2::DeleteBitmapProperties(CKBitmapProperties *bp) {
    ::CKDeletePointer(bp);
}

void CK2::ConvertEndianArray32(void *buf, int dwordCount) {
    ::CKConvertEndianArray32(buf, dwordCount);
}

void CK2::ConvertEndianArray16(void *buf, int dwordCount) {
    ::CKConvertEndianArray16(buf, dwordCount);
}

CKDWORD CK2::ConvertEndian32(CKDWORD dw) {
    return ::CKConvertEndian32(dw);
}

CKWORD CK2::ConvertEndian16(CKWORD w) {
    return ::CKConvertEndian16(w);
}

CKDWORD CK2::ComputeDataCRC(const char *data, int size, CKDWORD prevCRC) {
    return ::CKComputeDataCRC(const_cast<char *>(data), size, prevCRC);
}

char *CK2::PackData(const char *data, int size, int &newSize, int compressionLevel) {
    return ::CKPackData(const_cast<char *>(data), size, newSize, compressionLevel);
}

char *CK2::UnPackData(int destSize, char *srcBuffer, int srcSize) {
    return ::CKUnPackData(destSize, srcBuffer, srcSize);
}

void *CK2::Malloc(unsigned int n) {
    return ::mynew(n);
}

void CK2::Free(void *ptr) {
    return ::mydelete(ptr);
}

void *CK2::MallocAligned(int size, int align) {
    return ::VxNewAligned(size, align);
}

void CK2::FreeAligned(void *ptr) {
    ::VxDeleteAligned(ptr);
}

char *CK2::Strdup(const char *str) {
    return ::CKStrdup(const_cast<CKSTRING>(str));
}

char *CK2::Strupr(const char *str) {
    return ::CKStrupr(const_cast<CKSTRING>(str));
}

char *CK2::Strlwr(const char *str) {
    return ::CKStrlwr(const_cast<CKSTRING>(str));
}

char *CK2::GetStringBuffer(int size) {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetStringBuffer(size);
}

CKGUID CK2::GetSecureGuid() {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetSecureGuid();
}

CKDWORD CK2::GetStartOptions() {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetStartOptions();
}

WIN_HANDLE CK2::GetMainWindow() {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetMainWindow();
}

int CK2::GetSelectedRenderEngine() {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetSelectedRenderEngine();
}

const char *CK2::GetStartPath() const {
    return ::CKGetStartPath();
}

const char *CK2::GetPluginsPath() const {
    return ::CKGetPluginsPath();
}

CKDWORD CK2::GetVersion() const {
    return ::CKGetVersion();
}

void CK2::SetInterfaceMode(bool mode, CKUICALLBACKFCT callback, void *data) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetInterfaceMode(mode, callback, data);
}

bool CK2::IsInInterfaceMode() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsInInterfaceMode();
}

CKERROR CK2::OutputToConsole(const char *str, bool beep) {
    return (CKERROR) SendInterfaceMessage(CKUIM_OUTTOCONSOLE, beep, (CKDWORD) str);
}

CKERROR CK2::OutputToConsoleEx(const char *format, ...) {
    assert(m_CKContext != nullptr);
    va_list args;
        va_start(args, format);
    char *buf = m_CKContext->GetStringBuffer(256);
    vsprintf(buf, format, args);
        va_end(args);
    return OutputToConsole(buf, false);
}

CKERROR CK2::OutputToConsoleExBeep(const char *format, ...) {
    assert(m_CKContext != nullptr);
    va_list args;
        va_start(args, format);
    char *buf = m_CKContext->GetStringBuffer(256);
    vsprintf(buf, format, args);
        va_end(args);
    return OutputToConsole(buf, true);
}

CKDWORD CK2::SendInterfaceMessage(CKDWORD reason, CKDWORD param1, CKDWORD param2) {
    assert(m_CKContext != nullptr);
    return m_CKContext->SendInterfaceMessage(reason, param1, param2);
}

void CK2::SetVirtoolsVersion(CK_VIRTOOLS_VERSION ver, CKDWORD build) {
    assert(m_CKContext != nullptr);
    m_CKContext->SetVirtoolsVersion(ver, build);
}

int CK2::GetPVInformation() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->GetPVInformation();
}

bool CK2::IsInDynamicCreationMode() const {
    assert(m_CKContext != nullptr);
    return m_CKContext->IsInDynamicCreationMode();
}

void *CK2::GetUserData(size_t type) const {
    return m_UserData.GetData(type);
}

void *CK2::SetUserData(void *data, size_t type) {
    return m_UserData.SetData(data, type);
}

CK2::CK2() = default;
