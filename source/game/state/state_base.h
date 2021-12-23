#pragma once

struct StateBase
{
	virtual ~StateBase() {};
	// 初期処理
	virtual void Init() {};
	// 実行
	virtual void Exec() {};
	// State変更前処理
	virtual void BeforeChange() {}
	// State変更後処理
	virtual void AfterChange() {}
};

