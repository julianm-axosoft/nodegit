
NAN_METHOD(GitTree::GetEntries) {
  Nan::EscapableHandleScope scope;

  git_error_clear();

  { // lock master scope start
    git_tree *tree = Nan::ObjectWrap::Unwrap<GitTree>(info.This())->GetValue();
    // nodegit::LockMaster lockMaster(false, tree);

    size_t size = git_tree_entrycount(tree);
    Local<Array> result = Nan::New<Array>(size);

    for (size_t i = 0; i < size; i++) {
      const git_tree_entry *entry = git_tree_entry_byindex(tree, i);
      Nan::Set(result, Nan::New<Number>(i), GitTreeEntry::New(entry, true));
    }

    return info.GetReturnValue().Set(scope.Escape(result));
  }
}
