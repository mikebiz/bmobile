#include "BlxContentServiceData.h"
#include "PublishingPointSourceMs.h"

using namespace System::EnterpriseServices;
using namespace System::Threading;

#include "Utilities.h"

CPublishingPointSourceMs::CPublishingPointSourceMs( LPWSTR szConnectionString )
{
		m_ConnStr = Marshal::PtrToStringUni( static_cast<IntPtr>(szConnectionString ) );

        this->m_commandCollection = gcnew cli::array< ::System::Data::IDbCommand^  >( PPS_MAX_COMMAND_ENTRIES );

		this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ] = (gcnew ::System::Data::SqlClient::SqlCommand());

		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->CommandText = L"dbo.uspPublishingPointSourceEstablishVideo";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipVerLen", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTos", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTotalLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipId", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipOffset", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTtl", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipProtocol", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@udpLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@majorType", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@subType", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@fixedSizeSamples", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@temporalCompression", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@sampleSize", 
			::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@headerFormatType", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@headerFormatSize", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcSourceLeft", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcSourceTop", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcSourceRight", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcSourceBottom", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcTargetLeft", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcTargetTop", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcTargetRight", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@rcTargetBottom", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@dwBitRate", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@dwBitErrorRate", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@AvgTimePerFrame", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biSize", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biHeight", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biWidth", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biPlanes", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biBitCount", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biCompression", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biSizeImage", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biXPelsPerMeter", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biYPelsPerMeter", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biClrUsed", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@biClrImportant", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@MacAddress", 
                ::System::Data::SqlDbType::Char, MACADDRESS_LENGTH, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ] = (gcnew ::System::Data::SqlClient::SqlCommand());

		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->CommandText = L"dbo.uspPublishingPointSourceEstablishAudio";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipVerLen", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTos", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTotalLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipId", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipOffset", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTtl", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipProtocol", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@udpLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@wFormatTag", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@nChannels", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@nSamplesPerSec", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@nAvgBytesPerSec", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@nBlockAlign", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@wBitsPerSample", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@cbSize", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@MacAddress", 
                ::System::Data::SqlDbType::Char, MACADDRESS_LENGTH, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ]))->CommandText = L"dbo.uspPublishingPointSourceReadById";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PublishingPointId", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ]))->CommandText = L"dbo.uspPublishingPointSourceReadById";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PublishingPointId", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ]))->CommandText = L"dbo.uspPublishingPointSourceRemoveVideo";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PptParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ]))->CommandText = L"dbo.uspPublishingPointSourceRemoveAudio";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PptParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

		return;
}

CPublishingPointSourceMs::~CPublishingPointSourceMs(void)
{
// add code to iterate through array of commands and close all and delete objects...
	return;
}

DWORD CPublishingPointSourceMs::EstablishVideoSource(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
		//, ITransaction* pTxn
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
			(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SOURCE ]));

		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@ConferenceID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->publishingPointId );
		command->Parameters[ L"@ConferenceParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );
		command->Parameters[ L"@ipVerLen" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_verlen );
		command->Parameters[ L"@ipTos" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_tos );
		command->Parameters[ L"@ipTotalLength" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_totallength );
		command->Parameters[ L"@ipId" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_id );
		command->Parameters[ L"@ipOffset" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_offset );
		command->Parameters[ L"@ipTtl" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_ttl );
		command->Parameters[ L"@ipProtocol" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_protocol );
		command->Parameters[ L"@srcIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_srcaddr );
		command->Parameters[ L"@destIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_destaddr );
		command->Parameters[ L"@srcPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->src_portno );
		command->Parameters[ L"@destPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->dst_portno );
		command->Parameters[ L"@udpLength" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->udp_length );
		command->Parameters[ L"@majorType" ]->Value = 
			Utilities::FromGUID( pEstablishVideoSource->majortype );
		command->Parameters[ L"@subType" ]->Value = 
			Utilities::FromGUID( pEstablishVideoSource->subtype );
		command->Parameters[ L"@fixedSizeSamples" ]->Value = 
			static_cast<System::Int32>( pEstablishVideoSource->fixedSizeSamples );
		command->Parameters[ L"@temporalCompression" ]->Value = 
			static_cast<System::Int32>( pEstablishVideoSource->temporalCompression );
		command->Parameters[ L"@sampleSize" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->sampleSize );
		command->Parameters[ L"@headerFormatType" ]->Value = 
			Utilities::FromGUID( pEstablishVideoSource->headerFormatType );
		command->Parameters[ L"@headerFormatSize" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->headerFormatSize );
		command->Parameters[ L"@rcSourceLeft" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcSource.left );
		command->Parameters[ L"@rcSourceTop" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcSource.top );
		command->Parameters[ L"@rcSourceRight" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcSource.right );
		command->Parameters[ L"@rcSourceBottom" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcSource.bottom );
		command->Parameters[ L"@rcTargetLeft" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcTarget.left );
		command->Parameters[ L"@rcTargetTop" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcTarget.top );
		command->Parameters[ L"@rcTargetRight" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcTarget.right );
		command->Parameters[ L"@rcTargetBottom" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->rcTarget.bottom );
		command->Parameters[ L"@dwBitRate" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->dwBitRate );
		command->Parameters[ L"@dwBitErrorRate" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->dwBitErrorRate );
		command->Parameters[ L"@AvgTimePerFrame" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->AvgTimePerFrame );
		command->Parameters[ L"@biSize" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biSize );
		command->Parameters[ L"@biHeight" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biHeight );
		command->Parameters[ L"@biWidth" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biWidth );
		command->Parameters[ L"@biPlanes" ]->Value = 
			static_cast<System::Int32>( pEstablishVideoSource->biPlanes );
		command->Parameters[ L"@biBitCount" ]->Value = 
			static_cast<System::Int32>( pEstablishVideoSource->biBitCount );
		command->Parameters[ L"@biCompression" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biCompression );
		command->Parameters[ L"@biSizeImage" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biSizeImage );
		command->Parameters[ L"@biXPelsPerMeter" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biXPelsPerMeter );
		command->Parameters[ L"@biYPelsPerMeter" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biYPelsPerMeter );
		command->Parameters[ L"@biClrUsed" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biClrUsed );
		command->Parameters[ L"@biClrImportant" ]->Value = 
			static_cast<System::Int64>( pEstablishVideoSource->biClrImportant );
		command->Parameters[ L"@MacAddress" ]->Value = 
			Marshal::PtrToStringAnsi( (IntPtr)pEventHeader->eventMacAddress );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ContextUtil::SetAbort();
		}
		finally 
		{
//			TransactionStatus^ status = ServiceDomain::Leave();
////			SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}

	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

	return returnValue;
}

DWORD CPublishingPointSourceMs::EstablishAudioSource(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_AUDIO_SOURCE ]));

		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@ConferenceID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->publishingPointId );
		command->Parameters[ L"@ConferenceParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );
		command->Parameters[ L"@ipVerLen" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_verlen );
		command->Parameters[ L"@ipTos" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_tos );
		command->Parameters[ L"@ipTotalLength" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_totallength );
		command->Parameters[ L"@ipId" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_id );
		command->Parameters[ L"@ipOffset" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_offset );
		command->Parameters[ L"@ipTtl" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_ttl );
		command->Parameters[ L"@ipProtocol" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_protocol );
		command->Parameters[ L"@srcIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_srcaddr );
		command->Parameters[ L"@destIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_destaddr );
		command->Parameters[ L"@srcPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->src_portno );
		command->Parameters[ L"@destPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->dst_portno );
		command->Parameters[ L"@udpLength" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->udp_length );
		command->Parameters[ L"@wFormatTag" ]->Value = 
			static_cast<System::Int32>( pEstablishAudioSource->wFormatTag );
		command->Parameters[ L"@nChannels" ]->Value = 
			static_cast<System::Int32>( pEstablishAudioSource->nChannels );
		command->Parameters[ L"@nSamplesPerSec" ]->Value = 
			static_cast<System::Int64>( pEstablishAudioSource->nSamplesPerSec );
		command->Parameters[ L"@nAvgBytesPerSec" ]->Value = 
			static_cast<System::Int64>( pEstablishAudioSource->nAvgBytesPerSec );
		command->Parameters[ L"@nBlockAlign" ]->Value = 
			static_cast<System::Int32>( pEstablishAudioSource->nBlockAlign );
		command->Parameters[ L"@wBitsPerSample" ]->Value = 
			static_cast<System::Int32>( pEstablishAudioSource->wBitsPerSample );
		command->Parameters[ L"@cbSize" ]->Value = 
			static_cast<System::Int32>( pEstablishAudioSource->cbSize );
		command->Parameters[ L"@MacAddress" ]->Value = 
			Marshal::PtrToStringAnsi( (IntPtr)pEventHeader->eventMacAddress );


		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ContextUtil::SetAbort();
		}
		finally 
		{
			//ServiceDomain::Leave();
			////SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

	return returnValue;
}

DWORD CPublishingPointSourceMs::ReadVideoSource(
		GUID ID
		, PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_VIDEO_SOURCE pEstablishVideoSource
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SOURCES ]));

		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::Supported;
	
		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@PublishingPointId" ]->Value = 
			Utilities::FromGUID( ID );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );

			SqlDataReader^ reader = command->ExecuteReader();

			try
			{
				while( reader->Read() )
				{
					pIpHeader->ip_verlen			= static_cast<INT16>( reader[ L"ipVerLen" ] );
					pIpHeader->ip_tos				= static_cast<INT16>( reader[ L"ipTos" ] );
					pIpHeader->ip_totallength		= static_cast<INT32>( reader[ L"ipTotalLength" ] );
					pIpHeader->ip_id				= static_cast<INT32>( reader[ L"ipId" ] );
					pIpHeader->ip_offset			= static_cast<INT32>( reader[ L"ipOffset" ] );
					pIpHeader->ip_ttl				= static_cast<INT16>( reader[ L"ipTtl" ] );
					pIpHeader->ip_protocol			= static_cast<INT16>( reader[ L"ipProtocol" ] );
					pIpHeader->ip_srcaddr			= static_cast<INT64>( reader[ L"srcIP" ] );
					pIpHeader->ip_destaddr			= static_cast<INT64>( reader[ L"destIP" ] );

					pUdpHeader->src_portno			= static_cast<INT32>( reader[ L"srcPort" ] );
					pUdpHeader->dst_portno			= static_cast<INT32>( reader[ L"destPort" ] );
					pUdpHeader->udp_length			= static_cast<INT32>( reader[ L"udpLength" ] );
					pUdpHeader->udp_checksum		= 0;

					::CoCreateGuid( &pEventHeader->eventId ); 
					pEventHeader->eventType			= BE_ESTABLISH_VIDEO_SOURCE;
					pEventHeader->eventTypeId		= BET_MANAGEMENT;

					PCHAR str2 = (PCHAR)(PVOID)Marshal::StringToHGlobalAnsi( reader->GetString( reader->GetOrdinal( L"MacAddress" ) ) );
					strncpy_s(
						pEventHeader->eventMacAddress 
						, _countof( pEventHeader->eventMacAddress ) //- 1
						, str2
						, _TRUNCATE
						);
					Marshal::FreeHGlobal( static_cast<System::IntPtr>( str2 ) );

					::CoCreateGuid( &pEventPacketHeader->securityToken );
					pEventPacketHeader->eventWsaBufType = PDBT_EVENT_PACKET_PAYLOAD;
					pEventPacketHeader->eventPublishingPoint = static_cast<INT32>( reader[ L"SessionID" ] );
					pEventPacketHeader->publishingPointId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PublishingPointId" ) ) );
					pEventPacketHeader->pptParticipantId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PptParticipantId" ) ) );
					
					if( NULL != pEstablishVideoSource )
					{
						pEstablishVideoSource->majortype = 
							Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"majorType" ) ) );
						pEstablishVideoSource->subtype = 
							Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"subType" ) ) );
						pEstablishVideoSource->fixedSizeSamples = static_cast<INT32>( reader[ L"fixedSizeSamples" ] );
						pEstablishVideoSource->temporalCompression = static_cast<INT32>( reader[ L"temporalCompression" ] );
						pEstablishVideoSource->sampleSize = static_cast<INT64>( reader[ L"sampleSize" ] );
						pEstablishVideoSource->headerFormatType = 
							Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"headerFormatType" ) ) );
						pEstablishVideoSource->headerFormatSize = static_cast<INT64>( reader[ L"headerFormatSize" ] );

						pEstablishVideoSource->rcSource.left = static_cast<INT64>( reader[ L"rcSourceLeft" ] );
						pEstablishVideoSource->rcSource.top = static_cast<INT64>( reader[ L"rcSourceTop" ] );
						pEstablishVideoSource->rcSource.bottom = static_cast<INT64>( reader[ L"rcSourceBottom" ] );
						pEstablishVideoSource->rcSource.right = static_cast<INT64>( reader[ L"rcSourceRight" ] );
						pEstablishVideoSource->rcTarget.left = static_cast<INT64>( reader[ L"rcTargetLeft" ] );
						pEstablishVideoSource->rcTarget.top = static_cast<INT64>( reader[ L"rcTargetTop" ] );
						pEstablishVideoSource->rcTarget.bottom = static_cast<INT64>( reader[ L"rcTargetBottom" ] );
						pEstablishVideoSource->rcTarget.right = static_cast<INT64>( reader[ L"rcTargetRight" ] );

						pEstablishVideoSource->dwBitRate = static_cast<INT64>( reader[ L"dwBitRate" ] );
						pEstablishVideoSource->dwBitErrorRate = static_cast<INT64>( reader[ L"dwBitErrorRate" ] );
						pEstablishVideoSource->AvgTimePerFrame = static_cast<INT64>( reader[ L"AvgTimePerFrame" ] );
						pEstablishVideoSource->biSize = static_cast<INT64>( reader[ L"biSize" ] );
						pEstablishVideoSource->biHeight = static_cast<INT64>( reader[ L"biHeight" ] );
						pEstablishVideoSource->biWidth = static_cast<INT64>( reader[ L"biWidth" ] );
						pEstablishVideoSource->biPlanes = static_cast<INT32>( reader[ L"biPlanes" ] );
						pEstablishVideoSource->biBitCount = static_cast<INT32>( reader[ L"biBitCount" ] );
						pEstablishVideoSource->biCompression = static_cast<INT64>( reader[ L"biCompression" ] );
						pEstablishVideoSource->biSizeImage = static_cast<INT64>( reader[ L"biSizeImage" ] );
						pEstablishVideoSource->biXPelsPerMeter = static_cast<INT64>( reader[ L"biXPelsPerMeter" ] );
						pEstablishVideoSource->biYPelsPerMeter = static_cast<INT64>( reader[ L"biYPelsPerMeter" ] );
						pEstablishVideoSource->biClrUsed = static_cast<INT64>( reader[ L"biClrUsed" ] );
						pEstablishVideoSource->biClrImportant = static_cast<INT64>( reader[ L"biClrImportant" ] );

						pEventPacketHeader->dataBlobSize = sizeof( *pEstablishVideoSource );
					}
					else
					{
						pEventPacketHeader->dataBlobSize = 0;
					}

				}
			}
			finally
			{
				delete reader;
			}
		}
		finally 
		{
			////SqlConnection::ClearPool( command->Connection );
			//ServiceDomain::Leave();
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

    return returnValue;
}

DWORD CPublishingPointSourceMs::ReadAudioSource(
		GUID ID
		, PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_AUDIO_SOURCE pEstablishAudioSource
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_AUDIO_SOURCES ]));
	
		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::Supported;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@PublishingPointId" ]->Value = 
			Utilities::FromGUID( ID );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );

			SqlDataReader^ reader = command->ExecuteReader();

			try
			{
				while( reader->Read() )
				{
					pIpHeader->ip_verlen			= static_cast<INT16>( reader[ L"ipVerLen" ] );
					pIpHeader->ip_tos				= static_cast<INT16>( reader[ L"ipTos" ] );
					pIpHeader->ip_totallength		= static_cast<INT32>( reader[ L"ipTotalLength" ] );
					pIpHeader->ip_id				= static_cast<INT32>( reader[ L"ipId" ] );
					pIpHeader->ip_offset			= static_cast<INT32>( reader[ L"ipOffset" ] );
					pIpHeader->ip_ttl				= static_cast<INT16>( reader[ L"ipTtl" ] );
					pIpHeader->ip_protocol			= static_cast<INT16>( reader[ L"ipProtocol" ] );
					pIpHeader->ip_srcaddr			= static_cast<INT64>( reader[ L"srcIP" ] );
					pIpHeader->ip_destaddr			= static_cast<INT64>( reader[ L"destIP" ] );

					pUdpHeader->src_portno			= static_cast<INT32>( reader[ L"srcPort" ] );
					pUdpHeader->dst_portno			= static_cast<INT32>( reader[ L"destPort" ] );
					pUdpHeader->udp_length			= static_cast<INT32>( reader[ L"udpLength" ] );
					pUdpHeader->udp_checksum		= 0;

					::CoCreateGuid( &pEventHeader->eventId ); 
					pEventHeader->eventType			= BE_ESTABLISH_AUDIO_SOURCE;
					pEventHeader->eventTypeId		= BET_MANAGEMENT;

					PCHAR str2 = (PCHAR)(PVOID)Marshal::StringToHGlobalAnsi( reader->GetString( reader->GetOrdinal( L"MacAddress" ) ) );
					strncpy_s(
						pEventHeader->eventMacAddress 
						, _countof( pEventHeader->eventMacAddress ) //- 1
						, str2
						, _TRUNCATE
						);
					Marshal::FreeHGlobal( static_cast<System::IntPtr>( str2 ) );

					::CoCreateGuid( &pEventPacketHeader->securityToken );
					pEventPacketHeader->eventWsaBufType = PDBT_EVENT_PACKET_PAYLOAD;
					pEventPacketHeader->eventPublishingPoint = static_cast<INT32>( reader[ L"SessionID" ] );
					pEventPacketHeader->publishingPointId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PublishingPointId" ) ) );
					pEventPacketHeader->pptParticipantId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PptParticipantId" ) ) );
					pEventPacketHeader->dataBlobSize = MAX_ESTABLISH_AUDIO_SOURCE;
					
					if( NULL != pEstablishAudioSource )
					{
						pEstablishAudioSource->wFormatTag = static_cast<INT32>( reader[ L"wFormatTag" ] );
						pEstablishAudioSource->nChannels = static_cast<INT32>( reader[ L"nChannels" ] );
						pEstablishAudioSource->nSamplesPerSec = static_cast<INT64>( reader[ L"nSamplesPerSec" ] );
						pEstablishAudioSource->nAvgBytesPerSec = static_cast<INT64>( reader[ L"nAvgBytesPerSec" ] );
						pEstablishAudioSource->nBlockAlign = static_cast<INT32>( reader[ L"nBlockAlign" ] );
						pEstablishAudioSource->wBitsPerSample = static_cast<INT32>( reader[ L"wBitsPerSample" ] );
						pEstablishAudioSource->cbSize = static_cast<INT32>( reader[ L"cbSize" ] );
					}

				}
			}
			finally
			{
				delete reader;
			}
		}
		finally 
		{
			////SqlConnection::ClearPool( command->Connection );
			//ServiceDomain::Leave();
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}
    return returnValue;
}

DWORD CPublishingPointSourceMs::RemoveAudioSource(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_AUDIO_SOURCE ]));
	
		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@PptParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ContextUtil::SetAbort();
		}
		finally 
		{
			//ServiceDomain::Leave();
			////SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

	return returnValue;
}


DWORD CPublishingPointSourceMs::RemoveVideoSource(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		)
{
	DWORD dwErrorStatus = 0;

	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SOURCE ]));
	
		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@PptParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ServiceDomain::Leave();
			//ContextUtil::SetAbort();
		}
		finally 
		{
			//ServiceDomain::Leave();
			////SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

	return returnValue;
}
