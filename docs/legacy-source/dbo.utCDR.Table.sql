USE [BCS]
GO
/****** Object:  Table [dbo].[utCDR]    Script Date: 08/27/2008 14:35:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utCDR](
	[ID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[ConferenceParticipantID] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[CDRType] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[ParticipateType] [uniqueidentifier] NOT NULL DEFAULT (newid()),
	[StartTime] [datetime] NOT NULL DEFAULT (getdate()),
	[EndTime] [datetime] NOT NULL DEFAULT (getdate()),
	[timeStamp] [datetime] NOT NULL DEFAULT (getdate()),
	[mCostPerMin] [money] NOT NULL DEFAULT ((0.0)),
	[mPricePerMin] [money] NOT NULL DEFAULT ((0.0)),
	[mExtCost] [money] NOT NULL DEFAULT ((0.0)),
	[mExtPrice] [money] NOT NULL DEFAULT ((0.0)),
 CONSTRAINT [PK_utCDR] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utCDR]  WITH CHECK ADD  CONSTRAINT [FK_utCDRType_utCDR_U] FOREIGN KEY([CDRType])
REFERENCES [dbo].[utCDRType] ([ID])
GO
ALTER TABLE [dbo].[utCDR] CHECK CONSTRAINT [FK_utCDRType_utCDR_U]
GO
ALTER TABLE [dbo].[utCDR]  WITH CHECK ADD  CONSTRAINT [FK_utConferenceParticipant_utCDR_U] FOREIGN KEY([ConferenceParticipantID])
REFERENCES [dbo].[utConferenceParticipant] ([ID])
GO
ALTER TABLE [dbo].[utCDR] CHECK CONSTRAINT [FK_utConferenceParticipant_utCDR_U]
GO
ALTER TABLE [dbo].[utCDR]  WITH CHECK ADD  CONSTRAINT [FK_utParticipateType_utCDR_U] FOREIGN KEY([ParticipateType])
REFERENCES [dbo].[utParticipateType] ([ID])
GO
ALTER TABLE [dbo].[utCDR] CHECK CONSTRAINT [FK_utParticipateType_utCDR_U]
GO
