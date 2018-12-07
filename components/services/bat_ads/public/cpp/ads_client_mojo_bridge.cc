/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ads/public/cpp/ads_client_mojo_bridge.h"

#include <functional>

#include "base/bind.h"
#include "base/callback.h"
#include "base/containers/flat_map.h"
#include "bat/ads/ads.h"

using namespace std::placeholders;

namespace bat_ads {

namespace {

ads::URLRequestMethod ToAdsURLRequestMethod(int32_t method) {
  return (ads::URLRequestMethod)method;
}

base::flat_map<std::string, std::string> ToFlatMap(
    const std::map<std::string, std::string>& map) {
  base::flat_map<std::string, std::string> flat_map;
  for (const auto it : map) {
    flat_map[it.first] = it.second;
  }
  return flat_map;
}

int32_t ToMojomResult(ads::Result result) {
  return (int32_t)result;
}

}  // namespace

AdsClientMojoBridge::AdsClientMojoBridge(ads::AdsClient* ads_client)
    : ads_client_(ads_client) {}

AdsClientMojoBridge::~AdsClientMojoBridge() {}

bool AdsClientMojoBridge::IsAdsEnabled(bool* is_enabled) {
  *is_enabled = ads_client_->IsAdsEnabled();
  return true;
}

void AdsClientMojoBridge::IsAdsEnabled(IsAdsEnabledCallback callback) {
  std::move(callback).Run(ads_client_->IsAdsEnabled());
}

bool AdsClientMojoBridge::GetAdsLocale(std::string* out_locale) {
  *out_locale = ads_client_->GetAdsLocale();
  return true;
}

void AdsClientMojoBridge::GetAdsLocale(GetAdsLocaleCallback callback) {
  std::move(callback).Run(ads_client_->GetAdsLocale());
}

bool AdsClientMojoBridge::GetAdsPerHour(uint64_t* out_ads_per_hour) {
  *out_ads_per_hour = ads_client_->GetAdsPerHour();
  return true;
}

void AdsClientMojoBridge::GetAdsPerHour(GetAdsPerHourCallback callback) {
  std::move(callback).Run(ads_client_->GetAdsPerHour());
}

bool AdsClientMojoBridge::GetAdsPerDay(uint64_t* out_ads_per_day) {
  *out_ads_per_day = ads_client_->GetAdsPerDay();
  return true;
}

void AdsClientMojoBridge::GetAdsPerDay(GetAdsPerDayCallback callback) {
  std::move(callback).Run(ads_client_->GetAdsPerDay());
}

bool AdsClientMojoBridge::IsNetworkConnectionAvailable(bool* out_available) {
  *out_available = ads_client_->IsNetworkConnectionAvailable();
  return true;
}

void AdsClientMojoBridge::IsNetworkConnectionAvailable(
    IsNetworkConnectionAvailableCallback callback) {
  std::move(callback).Run(ads_client_->IsNetworkConnectionAvailable());
}

bool AdsClientMojoBridge::GenerateUUID(std::string* out_uuid) {
  *out_uuid = ads_client_->GenerateUUID();
  return true;
}

void AdsClientMojoBridge::GenerateUUID(GenerateUUIDCallback callback) {
  std::move(callback).Run(ads_client_->GenerateUUID());
}

bool AdsClientMojoBridge::GetSSID(std::string* out_ssid) {
  *out_ssid = ads_client_->GetSSID();
  return true;
}

void AdsClientMojoBridge::GetSSID(GetSSIDCallback callback) {
  std::move(callback).Run(ads_client_->GetSSID());
}

bool AdsClientMojoBridge::IsNotificationsAvailable(bool* out_available) {
  *out_available = ads_client_->IsNotificationsAvailable();
  return true;
}

void AdsClientMojoBridge::IsNotificationsAvailable(
    IsNotificationsAvailableCallback callback) {
  std::move(callback).Run(ads_client_->IsNotificationsAvailable());
}

bool AdsClientMojoBridge::SetTimer(uint64_t time_offset, uint32_t* out_timer_id) {
  *out_timer_id = ads_client_->SetTimer(time_offset);
  return true;
}

void AdsClientMojoBridge::SetTimer(uint64_t time_offset, SetTimerCallback callback) {
  std::move(callback).Run(ads_client_->SetTimer(time_offset));
}

bool AdsClientMojoBridge::LoadJsonSchema(const std::string& name, std::string* out_json) {
  *out_json = ads_client_->LoadJsonSchema(name);
  return true;
}

void AdsClientMojoBridge::LoadJsonSchema(const std::string& name,
                                      LoadJsonSchemaCallback callback) {
  std::move(callback).Run(ads_client_->LoadJsonSchema(name));
}

bool AdsClientMojoBridge::GetLocales(std::vector<std::string>* out_locales) {
  *out_locales = ads_client_->GetLocales();
  return true;
}

void AdsClientMojoBridge::GetLocales(GetLocalesCallback callback) {
  std::move(callback).Run(ads_client_->GetLocales());
}

void AdsClientMojoBridge::SetIdleThreshold(int32_t threshold) {
  ads_client_->SetIdleThreshold(threshold);
}

void AdsClientMojoBridge::KillTimer(uint32_t timer_id) {
  ads_client_->KillTimer(timer_id);
}

bool AdsClientMojoBridge::GetUrlComponents(const std::string& url,
                                        bool* result,
                                        std::string* out_components_json) {
  ads::UrlComponents components;
  *result = ads_client_->GetUrlComponents(url, &components);
  *out_components_json = components.ToJson();

  return true;
}

void AdsClientMojoBridge::GetUrlComponents(const std::string& url,
                                        GetUrlComponentsCallback callback) {
  ads::UrlComponents components;
  bool result = ads_client_->GetUrlComponents(url, &components);

  std::move(callback).Run(result, components.ToJson());
}

bool AdsClientMojoBridge::GetClientInfo(const std::string& client_info,
                                     std::string* out_client_info) {
  ads::ClientInfo info;
  info.FromJson(client_info);
  ads_client_->GetClientInfo(&info);
  *out_client_info = info.ToJson();
  return true;
}

void AdsClientMojoBridge::GetClientInfo(const std::string& client_info,
                                     GetClientInfoCallback callback) {
  ads::ClientInfo info;
  info.FromJson(client_info);
  ads_client_->GetClientInfo(&info);
  std::move(callback).Run(info.ToJson());
}

void AdsClientMojoBridge::EventLog(const std::string& json) {
  ads_client_->EventLog(json);
}

// static
void AdsClientMojoBridge::OnLoad(CallbackHolder<LoadCallback>* holder,
                              ads::Result result,
                              const std::string& value) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), std::move(value));
  delete holder;
}

void AdsClientMojoBridge::Load(const std::string& name, LoadCallback callback) {
  // this gets deleted in OnLoad
  auto* holder =
      new CallbackHolder<LoadCallback>(AsWeakPtr(), std::move(callback));
  ads_client_->Load(name, std::bind( AdsClientMojoBridge::OnLoad, holder, _1, _2));
}

// static
void AdsClientMojoBridge::OnSave(CallbackHolder<SaveCallback>* holder,
                              ads::Result result) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result));
  delete holder;
}

void AdsClientMojoBridge::Save(const std::string& name,
          const std::string& value,
          SaveCallback callback) {
  // this gets deleted in OnSave
  auto* holder =
      new CallbackHolder<SaveCallback>(AsWeakPtr(), std::move(callback));
  ads_client_->Save(name, value,
      std::bind( AdsClientMojoBridge::OnSave, holder, _1));
}

// static
void AdsClientMojoBridge::OnReset(CallbackHolder<ResetCallback>* holder,
                              ads::Result result) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result));
  delete holder;
}

void AdsClientMojoBridge::Reset(const std::string& name, ResetCallback callback) {
  // this gets deleted in OnSave
  auto* holder =
      new CallbackHolder<ResetCallback>(AsWeakPtr(), std::move(callback));
  ads_client_->Reset(name, std::bind( AdsClientMojoBridge::OnReset, holder, _1));
}

// static
void AdsClientMojoBridge::OnLoadUserModelForLocale(
    CallbackHolder<LoadUserModelForLocaleCallback>* holder,
    ads::Result result,
    const std::string& value) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), std::move(value));
  delete holder;
}

void AdsClientMojoBridge::LoadUserModelForLocale(
    const std::string& locale,
    LoadUserModelForLocaleCallback callback) {
  // this gets deleted in OnLoadUserModelForLocale
  auto* holder = new CallbackHolder<LoadUserModelForLocaleCallback>(
      AsWeakPtr(), std::move(callback));
  ads_client_->LoadUserModelForLocale(locale,
      std::bind( AdsClientMojoBridge::OnLoadUserModelForLocale, holder, _1, _2));
}

// static
void AdsClientMojoBridge::OnURLRequest(CallbackHolder<URLRequestCallback>* holder,
                      const int status_code,
                      const std::string& content,
                      const std::map<std::string, std::string>& headers) {
  if (holder->is_valid()) {
    std::move(holder->get()).Run(status_code, content, ToFlatMap(headers));
  }
  delete holder;
}

void AdsClientMojoBridge::URLRequest(const std::string& url,
                                  const std::vector<std::string>& headers,
                                  const std::string& content,
                                  const std::string& content_type,
                                  int32_t method,
                                  URLRequestCallback callback) {
  // this gets deleted in OnURLRequest
  auto* holder =
      new CallbackHolder<URLRequestCallback>(AsWeakPtr(), std::move(callback));
  ads_client_->URLRequest(url,
                          headers,
                          content,
                          content_type,
                          ToAdsURLRequestMethod(method),
                          std::bind( AdsClientMojoBridge::OnURLRequest,
                              holder, _1, _2, _3));
}

// static
void AdsClientMojoBridge::OnLoadSampleBundle(
    CallbackHolder<LoadSampleBundleCallback>* holder,
    ads::Result result,
    const std::string& value) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), std::move(value));
  delete holder;
}

void AdsClientMojoBridge::LoadSampleBundle(LoadSampleBundleCallback callback) {
  // this gets deleted in OnLoadSampleBundle
  auto* holder = new CallbackHolder<LoadSampleBundleCallback>(
      AsWeakPtr(), std::move(callback));
  ads_client_->LoadSampleBundle(
      std::bind( AdsClientMojoBridge::OnLoadSampleBundle, holder, _1, _2));
}

void AdsClientMojoBridge::ShowNotification(
    const std::string& notification_info) {
  auto info = std::make_unique<ads::NotificationInfo>();
  if (info->FromJson(notification_info))
    ads_client_->ShowNotification(std::move(info));
}

// static
void AdsClientMojoBridge::OnSaveBundleState(
    CallbackHolder<SaveBundleStateCallback>* holder,
    ads::Result result) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result));
  delete holder;
}

void AdsClientMojoBridge::SaveBundleState(const std::string& bundle_state_json,
                       SaveBundleStateCallback callback) {
  // this gets deleted in OnSaveBundleState
  auto* holder = new CallbackHolder<SaveBundleStateCallback>(
      AsWeakPtr(), std::move(callback));
  auto bundle_state = std::make_unique<ads::BundleState>();

  auto schema = ads_client_->LoadJsonSchema(ads::_bundle_schema_name);

  if (bundle_state->FromJson(bundle_state_json, schema)) {
    ads_client_->SaveBundleState(std::move(bundle_state),
        std::bind( AdsClientMojoBridge::OnSaveBundleState, holder, _1));
  } else {
    std::move(callback).Run(ToMojomResult(ads::Result::FAILED));
  }
}

// static
void AdsClientMojoBridge::OnGetAds(
    CallbackHolder<GetAdsCallback>* holder,
    ads::Result result,
    const std::string& region,
    const std::string& category,
    const std::vector<ads::AdInfo>& ad_info) {
  if (holder->is_valid()) {
    std::vector<std::string> ad_info_json;
    for (const auto it : ad_info) {
      ad_info_json.push_back(it.ToJson());
    }
    std::move(holder->get()).Run(
        ToMojomResult(result), region, category, ad_info_json);
  }
  delete holder;
}

void AdsClientMojoBridge::GetAds(const std::string& region,
                              const std::string& category,
                              GetAdsCallback callback) {
  // this gets deleted in OnSaveBundleState
  auto* holder = new CallbackHolder<GetAdsCallback>(
      AsWeakPtr(), std::move(callback));

  ads_client_->GetAds(region, category,
      std::bind( AdsClientMojoBridge::OnGetAds, holder, _1, _2, _3, _4));
}

}  // namespace bat_ads