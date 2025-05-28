/****** Object:  Table [dbo].[utPublishingPointSource]    Script Date: 08/27/2008 14:35:24 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utPublishingPointSource](
	[ID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utPublishing__ID__241E3C13]  DEFAULT (newid()),
	[ConferenceParticipantID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utPublish__Confe__2512604C]  DEFAULT (newid()),
	[avSourceType] [int] NOT NULL CONSTRAINT [DF_utPublishingPointSource_avStatus]  DEFAULT ((0)),
	[ipVerLen] [smallint] NOT NULL,
	[ipTos] [smallint] NOT NULL,
	[ipTotalLength] [int] NOT NULL,
	[ipId] [int] NOT NULL,
	[ipOffset] [int] NOT NULL,
	[ipTtl] [smallint] NOT NULL,
	[ipProtocol] [smallint] NOT NULL,
	[srcIP] [bigint] NOT NULL CONSTRAINT [DF__utPublish__srcIP__26068485]  DEFAULT ((0)),
	[destIP] [bigint] NOT NULL CONSTRAINT [DF__utPublish__destI__26FAA8BE]  DEFAULT ((0)),
	[srcPort] [int] NOT NULL CONSTRAINT [DF__utPublish__srcPo__27EECCF7]  DEFAULT ((0)),
	[destPort] [int] NOT NULL CONSTRAINT [DF__utPublish__destP__28E2F130]  DEFAULT ((0)),
	[udpLength] [int] NULL,
	[majorType] [uniqueidentifier] NULL CONSTRAINT [DF__utPublish__major__29D71569]  DEFAULT (newid()),
	[subType] [uniqueidentifier] NULL CONSTRAINT [DF__utPublish__subTy__2ACB39A2]  DEFAULT (newid()),
	[fixedSizeSamples] [int] NULL CONSTRAINT [DF__utPublish__fixed__2BBF5DDB]  DEFAULT ((0)),
	[temporalCompression] [int] NULL CONSTRAINT [DF__utPublish__tempo__2CB38214]  DEFAULT ((0)),
	[sampleSize] [bigint] NULL CONSTRAINT [DF__utPublish__sampl__2DA7A64D]  DEFAULT ((0)),
	[headerFormatType] [uniqueidentifier] NULL CONSTRAINT [DF__utPublish__heade__2E9BCA86]  DEFAULT (newid()),
	[headerFormatSize] [bigint] NULL CONSTRAINT [DF__utPublish__heade__2F8FEEBF]  DEFAULT ((0)),
	[rcSourceLeft] [bigint] NULL CONSTRAINT [DF__utPublish__rcSou__308412F8]  DEFAULT ((0)),
	[rcSourceTop] [bigint] NULL CONSTRAINT [DF__utPublish__rcSou__31783731]  DEFAULT ((0)),
	[rcSourceRight] [bigint] NULL CONSTRAINT [DF__utPublish__rcSou__326C5B6A]  DEFAULT ((0)),
	[rcSourceBottom] [bigint] NULL CONSTRAINT [DF__utPublish__rcSou__33607FA3]  DEFAULT ((0)),
	[rcTargetLeft] [bigint] NULL CONSTRAINT [DF__utPublish__rcTar__3454A3DC]  DEFAULT ((0)),
	[rcTargetTop] [bigint] NULL CONSTRAINT [DF__utPublish__rcTar__3548C815]  DEFAULT ((0)),
	[rcTargetRight] [bigint] NULL CONSTRAINT [DF__utPublish__rcTar__363CEC4E]  DEFAULT ((0)),
	[rcTargetBottom] [bigint] NULL CONSTRAINT [DF__utPublish__rcTar__37311087]  DEFAULT ((0)),
	[dwBitRate] [bigint] NULL CONSTRAINT [DF__utPublish__dwBit__382534C0]  DEFAULT ((0)),
	[dwBitErrorRate] [bigint] NULL CONSTRAINT [DF__utPublish__dwBit__391958F9]  DEFAULT ((0)),
	[AvgTimePerFrame] [bigint] NULL CONSTRAINT [DF__utPublish__AvgTi__3A0D7D32]  DEFAULT ((0)),
	[biSize] [bigint] NULL CONSTRAINT [DF__utPublish__biSiz__3B01A16B]  DEFAULT ((0)),
	[biHeight] [bigint] NULL CONSTRAINT [DF__utPublish__biHei__3BF5C5A4]  DEFAULT ((0)),
	[biWidth] [bigint] NULL CONSTRAINT [DF__utPublish__biWid__3CE9E9DD]  DEFAULT ((0)),
	[biPlanes] [int] NULL CONSTRAINT [DF__utPublish__biPla__3DDE0E16]  DEFAULT ((0)),
	[biBitCount] [int] NULL CONSTRAINT [DF__utPublish__biBit__3ED2324F]  DEFAULT ((0)),
	[biCompression] [bigint] NULL CONSTRAINT [DF__utPublish__biCom__3FC65688]  DEFAULT ((0)),
	[biSizeImage] [bigint] NULL CONSTRAINT [DF__utPublish__biSiz__40BA7AC1]  DEFAULT ((0)),
	[biXPelsPerMeter] [bigint] NULL CONSTRAINT [DF__utPublish__biXPe__41AE9EFA]  DEFAULT ((0)),
	[biYPelsPerMeter] [bigint] NULL CONSTRAINT [DF__utPublish__biYPe__42A2C333]  DEFAULT ((0)),
	[biClrUsed] [bigint] NULL CONSTRAINT [DF__utPublish__biClr__4396E76C]  DEFAULT ((0)),
	[biClrImportant] [bigint] NULL CONSTRAINT [DF__utPublish__biClr__448B0BA5]  DEFAULT ((0)),
	[wFormatTag] [int] NULL CONSTRAINT [DF__utPublish__wForm__457F2FDE]  DEFAULT ((0)),
	[nChannels] [int] NULL CONSTRAINT [DF__utPublish__nChan__46735417]  DEFAULT ((0)),
	[nSamplesPerSec] [bigint] NULL CONSTRAINT [DF__utPublish__nSamp__47677850]  DEFAULT ((0)),
	[nAvgBytesPerSec] [bigint] NULL CONSTRAINT [DF__utPublish__nAvgB__485B9C89]  DEFAULT ((0)),
	[nBlockAlign] [int] NULL CONSTRAINT [DF__utPublish__nBloc__494FC0C2]  DEFAULT ((0)),
	[wBitsPerSample] [int] NULL CONSTRAINT [DF__utPublish__wBits__4A43E4FB]  DEFAULT ((0)),
	[cbSize] [int] NULL CONSTRAINT [DF__utPublish__cbSiz__4B380934]  DEFAULT ((0)),
	[isActive] [bit] NULL CONSTRAINT [DF_utPublishingPointSource_isActive]  DEFAULT ((1)),
	[isStreamable] [bit] NULL CONSTRAINT [DF_utPublishingPointSource_isStreamable]  DEFAULT ((1)),
	[lastUpdated] [datetime] NULL CONSTRAINT [DF__utPublish__lastU__4C2C2D6D]  DEFAULT (getdate()),
 CONSTRAINT [PK_utPublishingPointSource] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utPublishingPointSource]  WITH CHECK ADD  CONSTRAINT [FK_utConferenceParticipant_utPublishingPointSource_U] FOREIGN KEY([ConferenceParticipantID])
REFERENCES [dbo].[utConferenceParticipant] ([ID])
GO
ALTER TABLE [dbo].[utPublishingPointSource] CHECK CONSTRAINT [FK_utConferenceParticipant_utPublishingPointSource_U]
GO
