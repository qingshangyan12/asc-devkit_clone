# HCCL Context Introduction

<!-- md-trans-meta sourceCommit=4c68db07c483e21d3d63ceb9ef38c2963da2162f translatedAt=2026-10-08T12:09:11.368Z pushedAt=2026-10-26T06:19:00.530Z -->

The interfaces in this chapter are used to set/obtain the context (message area) address corresponding to each communication domain of the communication-computation fusion operator on the kernel side. You must first call the [HcclGroup](../../../../Utils-API/prototype_register_management/OpMC2Def/HcclGroup.md) interface on the host side to configure the communication domain name before calling [GetHcclContext](GetHcclContext.md) to obtain the corresponding context address.

> [!NOTE]
> This interface is an experimental interface and may be adjusted or improved in later versions, without guaranteeing backward compatibility. Developers are advised to keep track of updates in later versions during use.

