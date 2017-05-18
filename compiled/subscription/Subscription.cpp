/*
 * This file is part of Adblock Plus <https://adblockplus.org/>,
 * Copyright (C) 2006-2017 eyeo GmbH
 *
 * Adblock Plus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * Adblock Plus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Adblock Plus.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <cstdlib>

#include "Subscription.h"
#include "DownloadableSubscription.h"
#include "UserDefinedSubscription.h"
#include "../StringMap.h"

namespace
{
  StringMap<Subscription*> knownSubscriptions(16);
}

Subscription::Subscription(Type type, const String& id)
    : mID(id), mType(type), mDisabled(false)
{
  annotate_address(this, "Subscription");
}

Subscription::~Subscription()
{
  knownSubscriptions.erase(mID);
}

Filter* Subscription::FilterAt(unsigned index)
{
  if (index >= mFilters.size())
    return nullptr;

  FilterPtr result(mFilters[index]);
  return result.release();
}

int Subscription::IndexOfFilter(Filter* filter)
{
  for (unsigned i = 0; i < mFilters.size(); i++)
    if (mFilters[i] == filter)
      return i;
  return -1;
}

OwnedString Subscription::Serialize() const
{
  OwnedString result(u"[Subscription]\nurl="_str);
  result.append(mID);
  result.append(u'\n');
  if (!mTitle.empty())
  {
    result.append(u"title="_str);
    result.append(mTitle);
    result.append(u'\n');
  }
  if (mDisabled)
    result.append(u"disabled=true\n"_str);

  return result;
}

OwnedString Subscription::SerializeFilters() const
{
  if (!mFilters.size())
    return OwnedString();

  OwnedString result(u"[Subscription filters]\n"_str);
  for (const auto& filter : mFilters)
  {
    // TODO: Escape [ characters
    result.append(filter->GetText());
    result.append(u'\n');
  }
  return result;
}

Subscription* Subscription::FromID(const String& id)
{
  if (id.empty())
  {
    // Generate a new random ID
    unsigned int seed = knownSubscriptions.size();
    OwnedString randomID(u"~user~000000"_str);
    do
    {
      int number = rand_r(&seed);
      for (int i = randomID.length() - 6; i < randomID.length(); i++)
      {
        randomID[i] = '0' + (number % 10);
        number /= 10;
      }
    } while (knownSubscriptions.find(randomID));
    return FromID(randomID);
  }

  auto knownSubscription = knownSubscriptions.find(id);
  if (knownSubscription)
  {
    knownSubscription->second->AddRef();
    return knownSubscription->second;
  }

  SubscriptionPtr subscription;
  if (!id.empty() && id[0] == '~')
    subscription = SubscriptionPtr(new UserDefinedSubscription(id), false);
  else
    subscription = SubscriptionPtr(new DownloadableSubscription(id), false);

  // This is a hack: we looked up the entry using id but create it using
  // subscription->mID. This works because both are equal at this point.
  // However, id refers to a temporary buffer which will go away.
  enter_context("Adding to known subscriptions");
  knownSubscription.assign(subscription->mID, subscription.get());
  exit_context();

  return subscription.release();
}