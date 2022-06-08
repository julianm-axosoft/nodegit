
struct _HandleParentTreeEntry {
    v8::Local<v8::Value> treeObj;
    const git_tree_entry * entry;
};

void _IterateTree(git_repository *repo, git_tree *tree, std::vector<_HandleParentTreeEntry> &entries) {
  size_t size = git_tree_entrycount(tree);
  v8::Local<v8::Value> treeObj = GitTree::New(tree, true);
  for (size_t i = 0; i < size; i++) {
    const git_tree_entry *entry = git_tree_entry_byindex(tree, i);
    auto filemode = git_tree_entry_filemode(entry);
    if (filemode == GIT_FILEMODE_BLOB || filemode == GIT_FILEMODE_BLOB_EXECUTABLE) {
      entries.push_back({treeObj, entry});
    }
    if (filemode == GIT_FILEMODE_TREE) {
      git_tree *subtree;
      git_tree_lookup(&subtree, repo, git_tree_entry_id(entry));
      _IterateTree(repo, subtree, entries);
    }
  }
}

NAN_METHOD(GitTree::GetAllEntries) {
  Nan::EscapableHandleScope scope;

  if (info.Length() == 0 || !info[0]->IsObject()) {
    return Nan::ThrowError("Repository repo is required.");
  }

  git_error_clear();

  git_tree * tree = Nan::ObjectWrap::Unwrap<GitTree>(info.This())->GetValue();
  git_repository * repo = Nan::ObjectWrap::Unwrap<GitRepository>(Nan::To<v8::Object>(info[0]).ToLocalChecked())->GetValue();

  std::vector<_HandleParentTreeEntry> entries;

  _IterateTree(repo, tree, entries);

  Local<Array> result = Nan::New<Array>(entries.size());

  for (size_t i = 0; i < entries.size(); i++) {
    v8::Local<v8::Value> entryValue = GitTreeEntry::New(entries[i].entry, true);
    const v8::Local<v8::Object> entryObj = entryValue.As<v8::Object>();

    v8::Local<v8::Context> context = Nan::GetCurrentContext();
    if (entryObj->Set(context, Nan::New<v8::String>("parent").ToLocalChecked(), entries[i].treeObj).IsJust() == false) {
      return Nan::ThrowError("Could not set property parent");
    }

    Nan::Set(result, Nan::New<Number>(i), entryValue);
  }

  return info.GetReturnValue().Set(scope.Escape(result));
}
